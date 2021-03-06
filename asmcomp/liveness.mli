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

(* $Id: liveness.mli 12058 2012-01-20 14:23:34Z frisch $ *)

(* Liveness analysis.
   Annotate mach code with the set of regs live at each point. *)

open Format

val fundecl: formatter -> Mach.fundecl -> unit
