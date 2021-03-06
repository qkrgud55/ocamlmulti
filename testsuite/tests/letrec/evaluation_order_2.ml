(* A variant of evaluation_order_1.ml where the side-effects
   are inside the blocks. Note that this changes the evaluation
   order, as y is considered recursive.
*)
type tree = Tree of tree list

let test =
  let rec x = (Tree [(print_endline "x"; y); z])
  and y = Tree (print_endline "y"; [])
  and z = Tree (print_endline "z"; [x])
  in
  match (x, y, z) with
    | (Tree [y1; z1], Tree[], Tree[x1]) ->
      assert (y1 == y);
      assert (z1 == z);
      assert (x1 == x)
    | _ ->
      assert false
