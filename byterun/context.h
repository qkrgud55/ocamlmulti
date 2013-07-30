/*************************************************************/
/*    Reentrant patch by phc                                 */
/*************************************************************/

#ifndef PHC_CONTEXT_H
#define PHC_CONTEXT_H

#include "misc.h"
#include "mlvalues.h"

#define NULL_CTX 0
#define MAX_TH 16

struct caml_ref_table {
  value **base;
  value **end;
  value **threshold;
  value **ptr;
  value **limit;
  asize_t size;
  asize_t reserve;
};

#ifndef IO_BUFFER_SIZE
#define IO_BUFFER_SIZE 65536
#endif

#if defined(_WIN32)
typedef __int64 file_offset;
extern __int64 _lseeki64(int, __int64, int);
#define lseek(fd,d,m) _lseeki64(fd,d,m)
#elif defined(HAS_OFF_T)
#include <sys/types.h>
typedef off_t file_offset;
#else
typedef long file_offset;
#endif


struct channel {
  int fd;                       /* Unix file descriptor */
  file_offset offset;           /* Absolute position of fd in the file */
  char * end;                   /* Physical end of the buffer */
  char * curr;                  /* Current position in the buffer */
  char * max;                   /* Logical end of the buffer (for input) */
  void * mutex;                 /* Placeholder for mutex (for systhreads) */
  struct channel * next, * prev;/* Double chaining of channels (flush_all) */
  int revealed;                 /* For Cash only */
  int old_revealed;             /* For Cash only */
  int refcount;                 /* For flush_all and for Cash */
  int flags;                    /* Bitfield */
  char buff[IO_BUFFER_SIZE];    /* The buffer itself */
};


typedef struct phc_global_context {
  /* minor_gc.h */ 
  char *caml_young_ptr;
  char *caml_young_limit;
  char *caml_young_base;
  char *caml_young_start;
  char *caml_young_end;

  int count_id;

  struct caml_ref_table caml_ref_table;
  struct caml_ref_table caml_weak_ref_table;

  int caml_in_minor_collection;

  intnat caml_globals_scanned;
  intnat caml_globals_inited;

  struct channel *caml_all_opened_channels;
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

void caml_enter_cond_lock(pctxt ctx);
CAMLextern void (*caml_phc_create_thread)(void *(*fn)(void*), void *arg);


#endif
