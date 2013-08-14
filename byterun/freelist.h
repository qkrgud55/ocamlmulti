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

/* $Id: freelist.h 12910 2012-09-10 09:52:09Z doligez $ */

/* Free lists of heap blocks. */

#ifndef CAML_FREELIST_H
#define CAML_FREELIST_H


#include "misc.h"
#include "mlvalues.h"
#include "context.h"

extern asize_t caml_fl_cur_size;     /* size in words */

char *caml_fl_allocate (mlsize_t);
char *caml_fl_allocate_r (pctxt,mlsize_t);
void caml_fl_init_merge (void);
void caml_fl_init_merge_r (pctxt);
void caml_fl_reset (void);
void caml_fl_reset_r (pctxt);
char *caml_fl_merge_block (char *);
char *caml_fl_merge_block_r (pctxt,char *);
void caml_fl_add_blocks (char *);
void caml_fl_add_blocks_r (pctxt,char *);
void caml_make_free_blocks (value *, mlsize_t, int, int);
void caml_make_free_blocks_r (pctxt,value *, mlsize_t, int, int);
void caml_set_allocation_policy (uintnat);
void caml_set_allocation_policy_r (pctxt,uintnat);


#endif /* CAML_FREELIST_H */
