/*************************************************************/
/*    Reentrant patch by phc                                 */
/*************************************************************/

#ifndef PHC_CONTEXT_H
#define PHC_CONTEXT_H

#include "misc.h"

#define NULL_CTX 0
#define MAX_TH 16

typedef struct phc_global_context {
  
  /* minor_gc.h */ 
  char *caml_young_ptr;
  char *caml_young_limit;
  char *caml_young_base;
  char *caml_young_start;
  char *caml_young_end;

  int count_id;
} phc_global_context;

typedef phc_global_context *pctxt;

// phc - main_ctx valid only in non-parallel mode
// inited once by startup 
// use main_ctx to call a reentrant function inside a normal function
extern int access_to_non_ctx;
extern pctxt main_ctx;
extern int num_th;

extern pctxt create_empty_context(void);
extern void destroy_context(pctxt);
extern void sync_with_global_vars(pctxt ctx);
extern void sync_with_context(pctxt ctx);

CAMLextern void (*caml_lock_phc_mutex_fptr)(void);
CAMLextern void (*caml_unlock_phc_mutex_fptr)(void);

CAMLextern void (*caml_phc_create_thread)(void *(*fn)(void*), void *arg);


#endif
