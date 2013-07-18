/*************************************************************/
/*    Reentrant patch by phc                                 */
/*************************************************************/

#include "misc.h"
#include "context.h"
#include "mlvalues.h"
#include "minor_gc.h"
#include <stdlib.h>

int access_to_non_ctx = 0;
pctxt main_ctx = NULL;

pctxt create_empty_context(void){
  phc_global_context* res = NULL;

  res = malloc(sizeof(phc_global_context));
//  printf("create_empty_context %p\n", (void*)res);

  res->caml_young_ptr     = NULL;
  res->caml_young_limit   = NULL;
  res->caml_young_base    = NULL;
  res->caml_young_start   = NULL;
  res->caml_young_end     = NULL;

  main_ctx = res;
  return res;
}

CAMLprim value caml_print_context(pctxt ctx){
  printf("caml_young_ptr     : %p\n", (void*)ctx->caml_young_ptr);
  printf("caml_young_limit   : %p\n", (void*)ctx->caml_young_limit);
  printf("caml_young_base    : %p\n", (void*)ctx->caml_young_base);
  printf("caml_young_start   : %p\n", (void*)ctx->caml_young_start);
  printf("caml_young_end     : %p\n", (void*)ctx->caml_young_end);
  return Val_unit;
}

void sync_with_global_vars(pctxt ctx){
  ctx->caml_young_ptr     = caml_young_ptr;
  ctx->caml_young_limit   = caml_young_limit;
  ctx->caml_young_base    = caml_young_base;
  ctx->caml_young_start   = caml_young_start;
  ctx->caml_young_end     = caml_young_end;
}

void sync_with_context(pctxt ctx){
  caml_young_ptr     = ctx->caml_young_ptr; 
  caml_young_limit   = ctx->caml_young_limit;
  caml_young_base    = ctx->caml_young_base;
  caml_young_start   = ctx->caml_young_start;  
  caml_young_end     = ctx->caml_young_end;
}

void destroy_context(pctxt ctxt){
  free(ctxt);
}
