/***********************************************************************/
/*                                                                     */
/*                                OCaml                                */
/*                                                                     */
/*         Manuel Serrano and Xavier Leroy, INRIA Rocquencourt         */
/*                                                                     */
/*  Copyright 2000 Institut National de Recherche en Informatique et   */
/*  en Automatique.  All rights reserved.  This file is distributed    */
/*  under the terms of the GNU Library General Public License, with    */
/*  the special exception on linking described in file ../LICENSE.     */
/*                                                                     */
/***********************************************************************/

/* $Id: custom.h 12000 2012-01-07 20:55:28Z lefessan $ */

#ifndef CAML_CUSTOM_H
#define CAML_CUSTOM_H


#ifndef CAML_NAME_SPACE
#include "compatibility.h"
#endif
#include "mlvalues.h"
#include "context.h"

/*
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
*/

#define custom_finalize_default NULL
#define custom_compare_default NULL
#define custom_hash_default NULL
#define custom_serialize_default NULL
#define custom_deserialize_default NULL
#define custom_compare_ext_default NULL

#define Custom_ops_val(v) (*((struct custom_operations **) (v)))

#ifdef __cplusplus
extern "C" {
#endif


CAMLextern value caml_alloc_custom(struct custom_operations * ops,
                                   uintnat size, /*size in bytes*/
                                   mlsize_t mem, /*resources consumed*/
                                   mlsize_t max  /*max resources*/);
CAMLextern value caml_alloc_custom_r(pctxt ctx, struct custom_operations * ops,
                                   uintnat size, /*size in bytes*/
                                   mlsize_t mem, /*resources consumed*/
                                   mlsize_t max  /*max resources*/);


CAMLextern void caml_register_custom_operations(struct custom_operations * ops);
CAMLextern void caml_register_custom_operations_r(pctxt ctx, struct custom_operations * ops);

CAMLextern int caml_compare_unordered;
  /* Used by custom comparison to report unordered NaN-like cases. */

/* <private> */
extern struct custom_operations * caml_find_custom_operations(char * ident);
extern struct custom_operations * caml_find_custom_operations_r(pctxt ctx, char * ident);
extern struct custom_operations *
          caml_final_custom_operations(void (*fn)(value));
extern struct custom_operations *
          caml_final_custom_operations_r(pctxt ctx, void (*fn)(value));

extern void caml_init_custom_operations(void);
extern void caml_init_custom_operations_r(pctxt ctx);
/* </private> */

#ifdef __cplusplus
}
#endif

#endif /* CAML_CUSTOM_H */
