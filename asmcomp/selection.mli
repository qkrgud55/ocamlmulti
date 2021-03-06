(***********************************************************************)
(*                                                                     *)
(*                                OCaml                                *)
(*                                                                     *)
(*            Xavier Leroy, projet Cristal, INRIA Rocquencourt         *)
(*                                                                     *)
(*  Copyright 1996 Institut National de Recherche en Informatique et   *)
(*  en Automatique.  All rights reserved.  This file is distributed    *)
(*  under the terms of the Q Public License version 1.0.               *)
(*                                                                     *)
(***********************************************************************)

(* $Id: selection.mli 11156 2011-07-27 14:17:02Z doligez $ *)

(* Selection of pseudo-instructions, assignment of pseudo-registers,
   sequentialization. *)

val fundecl: Cmm.fundecl -> Mach.fundecl
