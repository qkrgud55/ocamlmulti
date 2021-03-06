/***********************************************************************/
/*                                                                     */
/*                                OCaml                                */
/*                                                                     */
/*         Xavier Leroy and Damien Doligez, INRIA Rocquencourt         */
/*                                                                     */
/*  Copyright 1996 Institut National de Recherche en Informatique et   */
/*  en Automatique.  All rights reserved.  This file is distributed    */
/*  under the terms of the GNU Library General Public License, with    */
/*  the special exception on linking described in file ../LICENSE.     */
/*                                                                     */
/***********************************************************************/

/* $Id: startup.c 12227 2012-03-13 14:44:48Z xleroy $ */

/* Start-up code */

#include <stdio.h>
#include <stdlib.h>
#include "callback.h"
#include "backtrace.h"
#include "custom.h"
#include "debugger.h"
#include "fail.h"
#include "freelist.h"
#include "gc.h"
#include "gc_ctrl.h"
#include "intext.h"
#include "memory.h"
#include "misc.h"
#include "mlvalues.h"
#include "osdeps.h"
#include "printexc.h"
#include "stack.h"
#include "sys.h"
#include "natdynlink.h"
#include "context.h"
#ifdef HAS_UI
#include "ui.h"
#endif

extern int caml_parser_trace;
extern pctxt ctxl[MAX_TH];
extern pthread_t thl[MAX_TH];

CAMLexport header_t caml_atom_table[256];
char * caml_code_area_start, * caml_code_area_end;
int is_ctx = 0;

/* Initialize the atom table and the static data and code area limits. */

struct segment { char * begin; char * end; };

static void init_atoms(void)
{
  extern struct segment caml_data_segments[], caml_code_segments[];
  int i;
  struct code_fragment * cf;

  for (i = 0; i < 256; i++) {
    caml_atom_table[i] = Make_header(0, i, Caml_white);
  }
  if (caml_page_table_add(In_static_data,
                          caml_atom_table, caml_atom_table + 256) != 0)
    caml_fatal_error("Fatal error: not enough memory for the initial page table");

  for (i = 0; caml_data_segments[i].begin != 0; i++) {
    /* PR#5509: we must include the zero word at end of data segment,
       because pointers equal to caml_data_segments[i].end are static data. */
    if (caml_page_table_add(In_static_data,
                            caml_data_segments[i].begin,
                            caml_data_segments[i].end + sizeof(value)) != 0)
      caml_fatal_error("Fatal error: not enough memory for the initial page table");
  }

  caml_code_area_start = caml_code_segments[0].begin;
  caml_code_area_end = caml_code_segments[0].end;
  for (i = 1; caml_code_segments[i].begin != 0; i++) {
    if (caml_code_segments[i].begin < caml_code_area_start)
      caml_code_area_start = caml_code_segments[i].begin;
    if (caml_code_segments[i].end > caml_code_area_end)
      caml_code_area_end = caml_code_segments[i].end;
  }
  /* Register the code in the table of code fragments */
  cf = caml_stat_alloc(sizeof(struct code_fragment));
  cf->code_start = caml_code_area_start;
  cf->code_end = caml_code_area_end;
  cf->digest_computed = 0;
  caml_ext_table_init(&caml_code_fragments_table, 8);
  caml_ext_table_add(&caml_code_fragments_table, cf);
}

/* Configuration parameters and flags */

static uintnat percent_free_init = Percent_free_def;
static uintnat max_percent_free_init = Max_percent_free_def;
static uintnat minor_heap_init = Minor_heap_def;
static uintnat heap_chunk_init = Heap_chunk_def;
static uintnat heap_size_init = Init_heap_def;
static uintnat max_stack_init = Max_stack_def;

/* Parse the CAMLRUNPARAM variable */
/* The option letter for each runtime option is the first letter of the
   last word of the ML name of the option (see [stdlib/gc.mli]).
   Except for l (maximum stack size) and h (initial heap size).
*/
/* Note: option l is irrelevant to the native-code runtime. */

/* If you change these functions, see also their copy in byterun/startup.c */

static void scanmult (char *opt, uintnat *var)
{
  char mult = ' ';
  int val;
  sscanf (opt, "=%u%c", &val, &mult);
  sscanf (opt, "=0x%x%c", &val, &mult);
  switch (mult) {
  case 'k':   *var = (uintnat) val * 1024; break;
  case 'M':   *var = (uintnat) val * 1024 * 1024; break;
  case 'G':   *var = (uintnat) val * 1024 * 1024 * 1024; break;
  default:    *var = (uintnat) val; break;
  }
}

static void parse_camlrunparam(void)
{
  char *opt = getenv ("OCAMLRUNPARAM");
  uintnat p;

  if (opt == NULL) opt = getenv ("CAMLRUNPARAM");

  if (opt != NULL){
    while (*opt != '\0'){
      switch (*opt++){
      case 's': scanmult (opt, &minor_heap_init); break;
      case 'i': scanmult (opt, &heap_chunk_init); break;
      case 'h': scanmult (opt, &heap_size_init); break;
      case 'l': scanmult (opt, &max_stack_init); break;
      case 'o': scanmult (opt, &percent_free_init); break;
      case 'O': scanmult (opt, &max_percent_free_init); break;
      case 'v': scanmult (opt, &caml_verb_gc); break;
      case 'b': caml_record_backtrace(Val_true); break;
      case 'p': caml_parser_trace = 1; break;
      case 'a': scanmult (opt, &p); caml_set_allocation_policy (p); break;
      case 'c': is_ctx = 1; break;
      case 'm': scanmult (opt, &num_th); break;
      }
    }
  }
}

/* These are termination hooks used by the systhreads library */
struct longjmp_buffer caml_termination_jmpbuf;
void (*caml_termination_hook)(void *) = NULL;

extern value caml_start_program (void);
extern value caml_start_program_r (pctxt ctx);
extern void caml_init_ieee_floats (void);
extern void caml_init_signals (void);
extern void caml_init_signals_r (pctxt ctx);

void caml_start_program_r_wrapper(void *ctx){
  caml_start_program_r(ctx);
}

void allocate_caml_globals(pctxt ctx){
  value *_caml_globals;
  int i, j;
  value v, w;

  _caml_globals = (value*)caml_globals;
//printf("allocate_caml_globals ctx = %p _caml_globals = %p\n", ctx, _caml_globals);
//printf("%p %p %p\n", _caml_globals[0], _caml_globals[1], _caml_globals[2]);
//printf("%x %x %x\n", Hd_val(_caml_globals[0]),Hd_val(_caml_globals[1]),Hd_val(_caml_globals[2]));
//printf("%d %d %d\n", Wosize_val(_caml_globals[0]),Wosize_val(_caml_globals[1]),Wosize_val(_caml_globals[2]));

  i = 0;
  while (_caml_globals[i]!=0) 
    i++;
  ctx->caml_globals = malloc(sizeof(value)*(i+1));
  ctx->caml_globals_len = i;
// printf("ctx->caml_globals = %p\n", ctx->caml_globals);

  i = 0;
  while (_caml_globals[i]!=0){
    v = _caml_globals[i];
// printf("v = %p hd = %x wosize = %d\n", v, Hd_val(v), Wosize_val(v));
    w = malloc(sizeof(value)*(Wosize_val(v)+1));
    *(value*)w = Hd_val(v);
    w = Val_hp(w);
    *(ctx->caml_globals+i) = w;

  //  printf("ctx->caml_globals[%d] = %p wosize = %d\n", i, w, Wosize_val(w));
    i++;
  }
  *(ctx->caml_globals+i) = 0;
}

void caml_main(char **argv)
{
  char * exe_name;
#ifdef __linux__
  static char proc_self_exe[256];
#endif
  value res;
  char tos;
  pctxt ctx;

  caml_init_ieee_floats();
  caml_init_custom_operations();
#ifdef DEBUG
  caml_verb_gc = 63;
#endif
  caml_top_of_stack = &tos;
  parse_camlrunparam();

  if (num_th){
    int i;
    
    for (i=0; i<num_th; i++){
      ctxl[i] = create_empty_context();
      caml_init_gc_r (ctxl[i], minor_heap_init, heap_size_init, 
                      heap_chunk_init, percent_free_init, 
                      max_percent_free_init);
      allocate_caml_globals(ctxl[i]);
      printf("asmrun/startup.c i=%d ctxl[i]=%p num_th=%d\n",
             i, (void*)ctxl[i], num_th);
    }
  
    init_atoms();
    init_phc_ctx_key();
    caml_init_signals_r(NULL);
    caml_register_ctx(ctxl[0], 0);
    caml_debugger_init (); /* force debugger.o stub to be linked */
    exe_name = argv[0];
    if (exe_name == NULL) exe_name = "";
    if (caml_executable_name(proc_self_exe, sizeof(proc_self_exe)) == 0)
      exe_name = proc_self_exe;
    else
      exe_name = caml_search_exe_in_path(exe_name);

    caml_sys_init(exe_name, argv);
    if (sigsetjmp(caml_termination_jmpbuf.buf, 0)) {
      if (caml_termination_hook != NULL) caml_termination_hook(NULL);
      return;
    }

/*
    printf("startup caml_globals\n");
    for (i=0; caml_globals[i]!=0; i++){
      value v = caml_globals[i];
      printf("Tag %d  Size %d\n", Tag_val(v), Wosize_val(v));
    }
*/
    
    for (i=0; i<num_th; i++){
      // caml_phc_create_thread(&caml_start_program_r, ctxl[i]);
      // caml_enter_cond_lock(ctxl[i]);
      pthread_create(thl+i, NULL, &caml_start_program_r_wrapper, ctxl[i]);
    }

    for (i=0; i<num_th; i++){
      pthread_join(thl[i], NULL);
    }
  }
  else {  // num_th == 0

    if (is_ctx){
      ctx = create_empty_context();
      printf("asmrun/startup.c ctx = %p\n", (void*)ctx);
  
      sync_with_global_vars (ctx);
      caml_init_gc_r (ctx, minor_heap_init, heap_size_init, heap_chunk_init,
                      percent_free_init, max_percent_free_init);
      sync_with_context (ctx);
    }
    else
      caml_init_gc (minor_heap_init, heap_size_init, heap_chunk_init,
                    percent_free_init, max_percent_free_init);
  
    init_atoms();
    caml_init_signals();
    caml_debugger_init (); /* force debugger.o stub to be linked */
    exe_name = argv[0];
    if (exe_name == NULL) exe_name = "";
  #ifdef __linux__
    if (caml_executable_name(proc_self_exe, sizeof(proc_self_exe)) == 0)
      exe_name = proc_self_exe;
    else
      exe_name = caml_search_exe_in_path(exe_name);
  #else
    exe_name = caml_search_exe_in_path(exe_name);
  #endif
    caml_sys_init(exe_name, argv);
    if (sigsetjmp(caml_termination_jmpbuf.buf, 0)) {
      if (caml_termination_hook != NULL) caml_termination_hook(NULL);
      return;
    }
    if (is_ctx){
      res = caml_start_program_r(ctx);
    }
    else
      res = caml_start_program();
  }

  if (is_ctx){
    if (access_to_non_ctx)
      printf("access_to_non_ctx=%d need resolution\n", access_to_non_ctx);
  }
//  if (Is_exception_result(res))
//    caml_fatal_uncaught_exception(Extract_exception(res));
}

void caml_startup(char **argv)
{
  caml_main(argv);
}
