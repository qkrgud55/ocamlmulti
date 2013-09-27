(* $Id: tlength.ml 11123 2011-07-20 09:17:07Z doligez $

A testbed file for private type abbreviation definitions.

We test the Length module that implements positive integers.

*)

(* We can build a null length. *)
let l = Length.make 0;;


(* We cannot build a negative length. *)
try ignore (Length.make (-1)); assert false with
| Failure _ -> ()
;;


(* We can build a positive length. *)
let n = 3;;
let l3 = Length.make n in

(* and use the associated injection and projection functions. *)
let l4 = Length.make (Length.from l3 + Length.from l3) in

Printf.printf "%d + %d = " n n;
print_int (Length.from l4);;
