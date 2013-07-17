/*************************************************************/
/*    Reentrant patch by phc                                 */
/*************************************************************/


#include "context.h"
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

void destroy_context(pctxt ctxt){
  free(ctxt);
}
