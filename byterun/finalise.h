/***********************************************************************/
/*                                                                     */
/*                                OCaml                                */
/*                                                                     */
/*          Damien Doligez, projet Moscova, INRIA Rocquencourt         */
/*                                                                     */
/*  Copyright 2000 Institut National de Recherche en Informatique et   */
/*  en Automatique.  All rights reserved.  This file is distributed    */
/*  under the terms of the GNU Library General Public License, with    */
/*  the special exception on linking described in file ../LICENSE.     */
/*                                                                     */
/***********************************************************************/

/* $Id: finalise.h 11156 2011-07-27 14:17:02Z doligez $ */

#ifndef CAML_FINALISE_H
#define CAML_FINALISE_H

#include "roots.h"
#include "context.h"

void caml_final_update (void);
void caml_final_update_r (pctxt);
void caml_final_do_calls (void);
void caml_final_do_calls_r (pctxt);
void caml_final_do_strong_roots (scanning_action f);
void caml_final_do_strong_roots_r (pctxt, scanning_action f);
void caml_final_do_weak_roots (scanning_action f);
void caml_final_do_weak_roots_r (pctxt, scanning_action f);
void caml_final_do_young_roots (scanning_action f);
void caml_final_do_young_roots_r (pctxt ctx, scanning_action_r f);
void caml_final_empty_young (void);
void caml_final_empty_young_r (pctxt ctx);
value caml_final_register (value f, value v);
value caml_final_register_r (pctxt, value f, value v);

#endif /* CAML_FINALISE_H */
