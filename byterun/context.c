/*************************************************************/
/*    Reentrant patch by phc                                 */
/*************************************************************/


#include "context.h"
#include <stdlib.h>

int access_to_non_ctx = 0;
pctxt the_ctxt = NULL;
pctxt main_ctx = NULL;

pctxt create_empty_context(void){
  phc_global_context* res = NULL;

  res = malloc(sizeof(phc_global_context));

  res->caml_young_ptr     = NULL;
  res->caml_young_limit   = NULL;
  res->caml_young_base    = NULL;
  res->caml_young_start   = NULL;
  res->caml_young_end     = NULL;

  the_ctxt = res;
  return res;
}

void destroy_context(pctxt ctxt){
  free(ctxt);
}
