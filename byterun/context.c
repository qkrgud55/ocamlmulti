/*************************************************************/
/*    Reentrant patch by phc                                 */
/*************************************************************/

#include "misc.h"
#include "context.h"
#include "mlvalues.h"
#include "minor_gc.h"
#include <stdlib.h>
#include <pthread.h>

int access_to_non_ctx = 0;
pctxt main_ctx = NULL;
int num_th = 0;
static int count_th = 0;

CAMLexport void (*caml_lock_phc_mutex_fptr)(void) = NULL;
CAMLexport void (*caml_unlock_phc_mutex_fptr)(void) = NULL;

pctxt create_empty_context(void){
  phc_global_context* res = NULL;

  res = malloc(sizeof(phc_global_context));
//  printf("create_empty_context %p\n", (void*)res);

  res->caml_young_ptr     = NULL;
  res->caml_young_limit   = NULL;
  res->caml_young_base    = NULL;
  res->caml_young_start   = NULL;
  res->caml_young_end     = NULL;

  res->count_id = count_th++;

  main_ctx = res;

  return res;
}

CAMLprim value caml_lock_phc_mutex(pctxt ctx, value v){
  int res;
  printf("caml_lock_phc_mutex\n");
  caml_lock_phc_mutex_fptr();
  return Val_unit;
}

CAMLprim value caml_unlock_phc_mutex(pctxt ctx, value v){
  int res;
  printf("caml_unlock_phc_mutex\n");
  caml_unlock_phc_mutex_fptr();
  return Val_unit;
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

void sync_with_context(pctxt ctx){
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

void destroy_context(pctxt ctxt){
  free(ctxt);
}
