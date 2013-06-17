(***********************************************************************)
(*                                                                     *)
(*                                OCaml                                *)
(*                                                                     *)
(*          Jerome Vouillon, projet Cristal, INRIA Rocquencourt        *)
(*          OCaml port by John Malecki and Xavier Leroy                *)
(*                                                                     *)
(*  Copyright 1996 Institut National de Recherche en Informatique et   *)
(*  en Automatique.  All rights reserved.  This file is distributed    *)
(*  under the terms of the Q Public License version 1.0.               *)
(*                                                                     *)
(***********************************************************************)

(* $Id: lexer.mli 11156 2011-07-27 14:17:02Z doligez $ *)

val line: Lexing.lexbuf -> string
val lexeme: Lexing.lexbuf -> Parser.token
val argument: Lexing.lexbuf -> Parser.token
val line_argument: Lexing.lexbuf -> Parser.token
