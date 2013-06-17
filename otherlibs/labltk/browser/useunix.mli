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

(* $Id: useunix.mli 11156 2011-07-27 14:17:02Z doligez $ *)

(* Unix utilities *)

val get_files_in_directory : string -> string list
val is_directory : string -> bool
val concat : string -> string -> string
val get_directories_in_files : path:string -> string list -> string list
val subshell : cmd:string -> string list
