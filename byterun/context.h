/*************************************************************/
/*    Reentrant patch by phc                                 */
/*************************************************************/

#ifndef PHC_CONTEXT_H
#define PHC_CONTEXT_H

#include "misc.h"
#include "mlvalues.h"

#include <setjmp.h>

#define NULL_CTX 0
#define MAX_TH 16

// fail.h

#ifndef PHC_CTX_LOGNJUMP
#define PHC_CTX_LOGNJUMP
#ifdef POSIX_SIGNALS
struct longjmp_buffer {
  sigjmp_buf buf;
};
#else
struct longjmp_buffer {
  jmp_buf buf;
};
#define sigsetjmp(buf,save) setjmp(buf)
#define siglongjmp(buf,val) longjmp(buf,val)
#endif
#endif


// stack.h
typedef struct {
  uintnat retaddr;
  unsigned short frame_size;
  unsigned short num_live;
  unsigned short live_ofs[1];
} frame_descr;

// roots.c
typedef struct link_t {
  void *data;
  struct link_t *next;
} link_t;


// finalise.c
struct final {
  value fun;
  value val;
  int offset;
};

struct to_do {
  struct to_do *next;
  int size;
  struct final item[1];  /* variable size */
};

// globroots.c
struct global_root {
  value * root;                    /* the address of the root */
  struct global_root * forward[1]; /* variable-length array */
};

#define NUM_LEVELS 17

struct global_root_list {
  value * root;                 /* dummy value for layout compatibility */
  struct global_root * forward[NUM_LEVELS]; /* forward chaining */
  int level;                    /* max used level */
};

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

// callback.c
#define Named_value_size 13

struct named_value {
  value val;
  struct named_value * next;
  char name[1];
};

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

// freelist.c

/* The free-list is kept sorted by increasing addresses.
   This makes the merging of adjacent free blocks possible.
   (See [caml_fl_merge_block].)
*/

typedef struct {
  char *next_bp;   /* Pointer to the first byte of the next block. */
} block;

/* The sentinel can be located anywhere in memory, but it must not be
   adjacent to any heap object. */
typedef struct sentinel_t{
  value filler1; /* Make sure the sentinel is never adjacent to any block. */
  header_t h;
  value first_bp;
  value filler2; /* Make sure the sentinel is never adjacent to any block. */
} sentinel_t;

// compare.c

struct compare_item { value * v1, * v2; mlsize_t count; };

#define COMPARE_STACK_INIT_SIZE 256
#define COMPARE_STACK_MAX_SIZE (1024*1024)

// custom.c

struct custom_operations {
  char *identifier;
  void (*finalize)(value v);
  int (*compare)(value v1, value v2);
  intnat (*hash)(value v);
  void (*serialize)(value v,
                    uintnat * wsize_32,
                    uintnat * wsize_64);
  uintnat (*deserialize)(void * dst);
  int (*compare_ext)(value v1, value v2);
};

struct custom_operations_list {
  struct custom_operations * ops;
  struct custom_operations_list * next;
};

// fail.c
#define BOUND_MSG "index out of bounds"
#define BOUND_MSG_LEN (sizeof(BOUND_MSG) - 1)

typedef struct {
  header_t hdr;
  value exn;
  value arg;
} array_bound_error_bucket_t;

typedef struct {
  header_t hdr;
  char data[BOUND_MSG_LEN + sizeof(value)];
} array_bound_error_msg_t;// = { 0, BOUND_MSG };


#define FLP_MAX 1000

typedef struct phc_global_context {
  char *caml_young_ptr;
  char *caml_young_limit;
  char *caml_young_base;
  char *caml_young_start;
  char *caml_young_end;

  value *caml_globals;              // 40
  int caml_globals_len;

  intnat caml_globals_inited;       // 56
  intnat caml_globals_scanned;

  uintnat caml_last_return_address; // 72   /* not in OCaml code initially */ 
  char * caml_bottom_of_stack;      // 80   /* no stack initially */

  value * caml_gc_regs;             // 88

  int caml_backtrace_active;        // 96
  // phc todo
  char *caml_exception_pointer;     // 104 

  int count_id;

  struct caml_ref_table caml_ref_table;
  struct caml_ref_table caml_weak_ref_table;

  uint32 random_seed;
  int caml_in_minor_collection;

  struct channel *caml_all_opened_channels;
  struct named_value * named_value_table[Named_value_size];

  struct global_root_list caml_global_roots;
                  /* mutable roots, don't know whether old or young */
  struct global_root_list caml_global_roots_young;
                 /* generational roots pointing to minor or major heap */
  struct global_root_list caml_global_roots_old;
                  /* generational roots pointing to major heap */

  value oldify_todo_list;

  struct final *final_table;
  value final_old, final_young, final_size;

  struct to_do *to_do_hd;
  struct to_do *to_do_tl;

  int running_finalisation_function;

  asize_t caml_minor_heap_size;
  // major_gc.c
  uintnat caml_percent_free;
  uintnat caml_major_heap_increment;
  char *caml_heap_start;
  char *caml_gc_sweep_hp;
  int caml_gc_phase;        /* always Phase_mark, Phase_sweep, or Phase_idle */
  value *gray_vals;
  value *gray_vals_cur, *gray_vals_end;
  asize_t gray_vals_size;
  int heap_is_pure;   /* The heap is pure if the only gray objects
                                below [markhp] are also in [gray_vals]. */
  uintnat caml_allocated_words;
  uintnat caml_dependent_size, caml_dependent_allocated;
  double caml_extra_heap_resources;
  uintnat caml_fl_size_at_phase_change;
  
  char *markhp, *chunk, *limit;
  
  int caml_gc_subphase;     /* Subphase_{main,weak1,weak2,final} */
  value *weak_prev;
  
  #ifdef DEBUG
  unsigned long major_gc_counter; // phc todo = 0;
  #endif

  // weak.c
  value caml_weak_list_head;
  value weak_dummy;
  value caml_weak_none;

  // freelist.c
  sentinel_t sentinel; 
  
  char *fl_head;
  char *fl_prev; /* Current allocation pointer.         */
  /* Last block in the list.  Only valid just after [caml_fl_allocate] returns NULL. */
  char *fl_last; 
  /* Current insertion pointer.  Managed jointly with [sweep_slice]. */
  char *caml_fl_merge; 
  /* Number of words in the free list, including headers but not fragments. */
  asize_t caml_fl_cur_size; 
  char *flp [FLP_MAX];
  int flp_size; 
  char *beyond; 
  char *last_fragment;

  // gc_ctrl.c
  double caml_stat_minor_words;
  double caml_stat_promoted_words;
  double caml_stat_major_words;

  intnat caml_stat_minor_collections;
  intnat caml_stat_major_collections;
  intnat caml_stat_heap_size;
  intnat caml_stat_top_heap_size;
  intnat caml_stat_compactions;
  intnat caml_stat_heap_chunks;

  // signals.c
  volatile int caml_force_major_slice;

  // roots.c
  struct caml__roots_block *caml_local_roots;
  void (*caml_scan_roots_hook) (scanning_action_r);

  int caml_frame_descr_inited;
  frame_descr ** caml_frame_descriptors;
  int caml_frame_descriptors_mask;

  link_t *frametables;

  char * caml_top_of_stack;
  link_t * caml_dyn_globals;

  uintnat (*caml_stack_usage_hook)(struct phc_global_context *);

  // compact.c
  char *compact_fl;
  uintnat caml_percent_max;

  // backtrace.c
  int caml_backtrace_pos;
  code_t * caml_backtrace_buffer;
  value caml_backtrace_last_exn;

  // meta.c
  value * caml_stack_low;
  value * caml_stack_high;
  value * caml_stack_threshold;
  value * caml_extern_sp;
  value * caml_trapsp;
  int caml_callback_depth;
  int volatile caml_something_to_do;
  void (*volatile caml_async_action_hook)(void);
  struct longjmp_buffer * caml_external_raise;

  // compare.c
  struct compare_item compare_stack_init[COMPARE_STACK_INIT_SIZE];
  struct compare_item *compare_stack;
  struct compare_item *compare_stack_limit;
  int caml_compare_unordered;

  // custom.c
  struct custom_operations_list * custom_ops_table;
  struct custom_operations_list * custom_ops_final_table;

  // fail.c
  array_bound_error_bucket_t  array_bound_error_bucket;
  array_bound_error_msg_t     array_bound_error_msg;
  int array_bound_error_bucket_inited;

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
extern void caml_phc_io_lock(void);
extern void caml_phc_io_unlock(void);

void caml_enter_cond_lock(pctxt ctx);
CAMLextern void (*caml_phc_create_thread)(void *(*fn)(void*), void *arg);
void init_phc_ctx_key(void);
pctxt get_ctx(void);


#endif
