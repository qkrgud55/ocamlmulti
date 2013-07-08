/*************************************************************/
/*    Reentrant patch by phc                                 */
/*************************************************************/

#ifndef PHC_CONTEXT_H
#define PHC_CONTEXT_H


#define NULL_CTX 0

typedef struct phc_global_context {
  
  /* minor_gc.h */ 
  char *caml_young_ptr;
  char *caml_young_limit;
  char *caml_young_base;
  char *caml_young_start;
  char *caml_young_end;
} phc_global_context;

typedef phc_global_context *pctxt;

extern pctxt the_ctxt;

extern pctxt create_empty_context(void);
extern void destroy_context(pctxt);

#endif
