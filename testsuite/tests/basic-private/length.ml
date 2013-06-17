(* $Id: length.ml 11123 2011-07-20 09:17:07Z doligez $

A testbed file for private type abbreviation definitions.

We define a Length module to implement positive integers.

*)

type t = int;;

let make x =
  if x >= 0 then x else
  failwith (Printf.sprintf "cannot build negative length : %i" x)
;;

external from : t -> int = "%identity";;
