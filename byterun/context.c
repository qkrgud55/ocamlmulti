/*************************************************************/
/*    Reentrant patch by phc                                 */
/*************************************************************/

#include "misc.h"
#include "context.h"
#include "mlvalues.h"
#include "minor_gc.h"
#include <stdlib.h>
#include <pthread.h>
#include "gc.h"

int access_to_non_ctx = 0;
pctxt main_ctx = NULL;
int num_th = 0;
static int count_th = 0;
static int count_start = 0;
static pthread_mutex_t phc_mutex_;
static pthread_mutex_t phc_cond_lock;
static pthread_cond_t phc_cond_;

CAMLexport void (*caml_lock_phc_mutex_fptr)(void) = NULL;
CAMLexport void (*caml_unlock_phc_mutex_fptr)(void) = NULL;
CAMLexport void (*caml_phc_create_thread)(void *(*fn)(void*), void *arg) = NULL;

pctxt create_empty_context(void){
  phc_global_context* res = NULL;
  phc_global_context _zero = {0,};
  struct caml_ref_table _ref_table = { NULL, NULL, NULL, NULL, NULL, 0, 0};
  struct global_root_list _global_roots = { NULL, { NULL, }, 0 };
  sentinel_t _sentinel = {0, Make_header (0, 0, Caml_blue), 0, 0};
  int i;

  res = malloc(sizeof(phc_global_context));
  *res = _zero;

  res->caml_young_ptr     = NULL;
  res->caml_young_limit   = NULL;
  res->caml_young_base    = NULL;
  res->caml_young_start   = NULL;
  res->caml_young_end     = NULL;

  res->count_id = count_th++;
 
  res->caml_ref_table = res->caml_weak_ref_table = _ref_table; 

  res->caml_in_minor_collection = 0;

  res->caml_globals_scanned = 0;
  res->caml_globals_inited = 0;

  res->caml_all_opened_channels = NULL;
  for (i=0; i<Named_value_size; i++)
    res->named_value_table[i] = NULL;

  res->caml_global_roots = _global_roots;
  res->caml_global_roots_young = _global_roots;
  res->caml_global_roots_old = _global_roots;

  res->random_seed = 0;

  res->oldify_todo_list = 0;
  res->final_table      = NULL;
  res->final_old        = 0;
  res->final_young      = 0;
  res->final_size       = 0;
  
  res->to_do_hd                      = NULL;
  res->to_do_tl                      = NULL;

  res->running_finalisation_function = 0;
  res->caml_fl_size_at_phase_change  = 0;

  #ifdef DEBUG
  ctx->major_gc_counter = 0;
  #endif

  res->caml_weak_list_head   = 0;
  res->weak_dummy            = 0;
  res->caml_weak_none        = (value)&(res->weak_dummy);

  res->sentinel         = _sentinel;
  res->fl_head          = ((char *) (&(res->sentinel.first_bp)));
  res->fl_prev          = res->fl_head;
  res->fl_last          = NULL;
  res->caml_fl_merge    = res->fl_prev;
  res->caml_fl_cur_size = 0;
  res->flp_size         = 0;
  res->beyond           = NULL;

  res->caml_stat_minor_words       = 0.0;
  res->caml_stat_promoted_words    = 0.0;
  res->caml_stat_major_words       = 0.0;

  res->caml_stat_minor_collections = 0;
  res->caml_stat_major_collections = 0;
  res->caml_stat_heap_size         = 0;
  res->caml_stat_top_heap_size     = 0;
  res->caml_stat_compactions       = 0;
  res->caml_stat_heap_chunks       = 0;


  res-> caml_last_return_address = 1;

  if (main_ctx==NULL){
    main_ctx = res;
    pthread_mutex_init(&phc_mutex_, NULL);
    pthread_mutex_init(&phc_cond_lock, NULL);
    pthread_cond_init(&phc_cond_, NULL);
  }
  return res;
}

CAMLprim value caml_lock_phc_mutex(pctxt ctx, value v){
  pthread_mutex_lock(&phc_mutex_);
  return Val_unit;
}

CAMLprim value caml_unlock_phc_mutex(pctxt ctx, value v){
  pthread_mutex_unlock(&phc_mutex_);
  return Val_unit;
}

void caml_enter_cond_lock(pctxt ctx){
  pthread_mutex_lock(&phc_mutex_);
  main_ctx = ctx;
  sync_with_context(ctx); 
}

CAMLprim value caml_wait_counter(pctxt ctx, value v){
// increase phc_count and wait for it to reach num_th 
// thus this fun blocks until it is OK to start parallel threading
// assumption :  pthread_mutex_lock(phc_mutex_);
  count_start++;
  pthread_mutex_unlock(&phc_mutex_);

  pthread_mutex_lock(&phc_cond_lock);
  while (count_start < num_th)
    pthread_cond_wait(&phc_cond_, &phc_cond_lock);
  pthread_cond_signal(&phc_cond_);
  pthread_mutex_unlock(&phc_cond_lock);
}

CAMLprim value caml_context_id(pctxt ctx, value v){
  return Val_int(ctx->count_id);
}

CAMLprim value caml_context_num(pctxt ctx, value v){
  return Val_int(count_th);
}

CAMLprim value caml_print_context(pctxt ctx){
  printf("caml_young_ptr     : %p\n", (void*)ctx->caml_young_ptr);
  printf("caml_young_limit   : %p\n", (void*)ctx->caml_young_limit);
  printf("caml_young_base    : %p\n", (void*)ctx->caml_young_base);
  printf("caml_young_start   : %p\n", (void*)ctx->caml_young_start);
  printf("caml_young_end     : %p\n", (void*)ctx->caml_young_end);
  return Val_unit;
}

void print_inconsis(char *name, value v1, value v2){
  printf("%s %p %p\n", name, v1, v2);
}

void sync_with_global_vars(pctxt ctx){
  if (num_th==0){

  if (ctx->caml_young_ptr!=caml_young_ptr)
    print_inconsis("sync_with_global_vars caml_young_ptr",
           ctx->caml_young_ptr, caml_young_ptr);
  if (ctx->caml_young_limit!=caml_young_limit)
    print_inconsis("sync_with_global_vars caml_young_limit",
           ctx->caml_young_limit, caml_young_limit);
  if (ctx->caml_young_base!=caml_young_base)
    print_inconsis("sync_with_global_vars caml_young_base",
           ctx->caml_young_base, caml_young_base);
  if (ctx->caml_young_start!=caml_young_start)
    print_inconsis("sync_with_global_vars caml_young_start",
           ctx->caml_young_start, caml_young_start);
  if (ctx->caml_young_end!=caml_young_end)
    print_inconsis("sync_with_global_vars caml_young_end",
           ctx->caml_young_end, caml_young_end);

  ctx->caml_young_ptr     = caml_young_ptr;
  ctx->caml_young_limit   = caml_young_limit;
  ctx->caml_young_base    = caml_young_base;
  ctx->caml_young_start   = caml_young_start;
  ctx->caml_young_end     = caml_young_end;

  }
}

void sync_with_context(pctxt ctx){
  if (num_th==0){

  if (ctx->caml_young_ptr!=caml_young_ptr)
    print_inconsis("sync_with_context caml_young_ptr",
           ctx->caml_young_ptr, caml_young_ptr);
  if (ctx->caml_young_limit!=caml_young_limit)
    print_inconsis("sync_with_context caml_young_limit",
           ctx->caml_young_limit, caml_young_limit);
  if (ctx->caml_young_base!=caml_young_base)
    print_inconsis("sync_with_context caml_young_base",
           ctx->caml_young_base, caml_young_base);
  if (ctx->caml_young_start!=caml_young_start)
    print_inconsis("sync_with_context caml_young_start",
           ctx->caml_young_start, caml_young_start);
  if (ctx->caml_young_end!=caml_young_end)
    print_inconsis("sync_with_context caml_young_end",
           ctx->caml_young_end, caml_young_end);

  caml_young_ptr     = ctx->caml_young_ptr; 
  caml_young_limit   = ctx->caml_young_limit;
  caml_young_base    = ctx->caml_young_base;
  caml_young_start   = ctx->caml_young_start;  
  caml_young_end     = ctx->caml_young_end;

  }
}

void destroy_context(pctxt ctxt){
  free(ctxt);
}
