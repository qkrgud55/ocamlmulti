/***********************************************************************/
/*                                                                     */
/*                                OCaml                                */
/*                                                                     */
/*             Damien Doligez, projet Para, INRIA Rocquencourt         */
/*                                                                     */
/*  Copyright 1996 Institut National de Recherche en Informatique et   */
/*  en Automatique.  All rights reserved.  This file is distributed    */
/*  under the terms of the GNU Library General Public License, with    */
/*  the special exception on linking described in file ../LICENSE.     */
/*                                                                     */
/***********************************************************************/

/* $Id: minor_gc.h 11156 2011-07-27 14:17:02Z doligez $ */

#ifndef CAML_MINOR_GC_H
#define CAML_MINOR_GC_H


#include "misc.h"
#include "context.h"

CAMLextern char *caml_young_start, *caml_young_ptr, *caml_young_base;
CAMLextern char *caml_young_end, *caml_young_limit;
extern asize_t caml_minor_heap_size;
extern int caml_in_minor_collection;


CAMLextern struct caml_ref_table caml_ref_table, caml_weak_ref_table;

#define Is_young(val) \
  (Assert (Is_block (val)), \
   (addr)(val) < (addr)caml_young_end && (addr)(val) > (addr)caml_young_start)

#define Is_young_r(ctx, val) \
  (Assert (Is_block (val)), \
   (addr)(val) < (addr)ctx->caml_young_end && (addr)(val) > (addr)ctx->caml_young_start)

extern void caml_set_minor_heap_size (asize_t);
extern void caml_set_minor_heap_size_r (pctxt, asize_t);
extern void caml_empty_minor_heap (void);
extern void caml_empty_minor_heap_r (pctxt);
CAMLextern void caml_minor_collection (void);
CAMLextern void caml_minor_collection_r (pctxt);

// phc - not used 
CAMLextern void garbage_collection (void); /* def in asmrun/signals.c */
extern void caml_realloc_ref_table (struct caml_ref_table *);
extern void caml_realloc_ref_table_r (pctxt, struct caml_ref_table *);
extern void caml_alloc_table (struct caml_ref_table *, asize_t, asize_t);
extern void caml_oldify_one (value, value *);
extern void caml_oldify_one_r (pctxt, value, value *);
extern void caml_oldify_mopup (void);
extern void caml_oldify_mopup_r (pctxt);

#define Oldify(p) do{ \
    value __oldify__v__ = *p; \
    if (Is_block (__oldify__v__) && Is_young (__oldify__v__)){ \
      caml_oldify_one (__oldify__v__, (p)); \
    } \
  }while(0)

#define Oldify_r(ctx, p) do{ \
    value __oldify__v__ = *p; \
    if (Is_block (__oldify__v__) && Is_young_r (ctx, __oldify__v__)){ \
      caml_oldify_one_r (ctx, __oldify__v__, (p)); \
    } \
  }while(0)


#endif /* CAML_MINOR_GC_H */
