/*************************************************************/
/*    Reentrant patch by phc                                 */
/*************************************************************/

#include "misc.h"
#include "context.h"
#include "mlvalues.h"
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

void destroy_context(pctxt ctxt){
  free(ctxt);
}
