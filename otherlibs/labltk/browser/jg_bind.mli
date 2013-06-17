(*************************************************************************)
(*                                                                       *)
(*                         OCaml LablTk library                          *)
(*                                                                       *)
(*            Jacques Garrigue, Kyoto University RIMS                    *)
(*                                                                       *)
(*   Copyright 1999 Institut National de Recherche en Informatique et    *)
(*   en Automatique and Kyoto University.  All rights reserved.          *)
(*   This file is distributed under the terms of the GNU Library         *)
(*   General Public License, with the special exception on linking       *)
(*   described in file ../../../LICENSE.                                 *)
(*                                                                       *)
(*************************************************************************)

(* $Id: jg_bind.mli 11156 2011-07-27 14:17:02Z doligez $ *)

open Widget

val enter_focus : 'a widget -> unit
val escape_destroy : ?destroy:'a widget -> 'a widget ->unit
val return_invoke : 'a widget -> button:button widget -> unit
