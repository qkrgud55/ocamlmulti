(***********************************************************************)
(*                                                                     *)
(*                                OCaml                                *)
(*                                                                     *)
(*            Damien Doligez, projet Para, INRIA Rocquencourt          *)
(*                                                                     *)
(*  Copyright 1996 Institut National de Recherche en Informatique et   *)
(*  en Automatique.  All rights reserved.  This file is distributed    *)
(*  under the terms of the GNU Library General Public License, with    *)
(*  the special exception on linking described in file ../LICENSE.     *)
(*                                                                     *)
(***********************************************************************)

(* $Id: gc.ml 11156 2011-07-27 14:17:02Z doligez $ *)

type stat = {
  minor_words : float;
  promoted_words : float;
  major_words : float;
  minor_collections : int;
  major_collections : int;
  heap_words : int;
  heap_chunks : int;
  live_words : int;
  live_blocks : int;
  free_words : int;
  free_blocks : int;
  largest_free : int;
  fragments : int;
  compactions : int;
  top_heap_words : int;
  stack_size : int;
};;

type control = {
  mutable minor_heap_size : int;
  mutable major_heap_increment : int;
  mutable space_overhead : int;
  mutable verbose : int;
  mutable max_overhead : int;
  mutable stack_limit : int;
  mutable allocation_policy : int;
};;

external stat : unit -> stat = "caml_gc_stat";;
external quick_stat : unit -> stat = "caml_gc_quick_stat";;
external counters : unit -> (float * float * float) = "caml_gc_counters";;
external get : unit -> control = "caml_gc_get";;
external get_r : unit -> control = "caml_gc_get" "reentrant";;
external set : control -> unit = "caml_gc_set";;
external set_r : control -> unit = "caml_gc_set_r" "reentrant";;
external minor : unit -> unit = "caml_gc_minor";;
external minor_r : unit -> unit = "caml_gc_minor_r" "reentrant";;
external major_slice : int -> int = "caml_gc_major_slice";;
external major : unit -> unit = "caml_gc_major";;
external full_major : unit -> unit = "caml_gc_full_major";;
external compact : unit -> unit = "caml_gc_compaction";;

external phc_context : unit -> unit = "caml_print_context" "reentrant";;
external lock_phc_mutex : unit -> unit = "caml_lock_phc_mutex" "reentrant";;
external unlock_phc_mutex : unit -> unit = "caml_unlock_phc_mutex" "reentrant";;
external get_th_id : unit -> int = "caml_context_id" "reentrant";;
external get_th_num : unit -> int = "caml_context_num" "reentrant";;
external end_perv_mode : unit -> int = "caml_wait_counter" "reentrant";;

open Printf;;

let print_stat c =
  let st = stat () in
  fprintf c "minor_words: %.0f\n" st.minor_words;
  fprintf c "promoted_words: %.0f\n" st.promoted_words;
  fprintf c "major_words: %.0f\n" st.major_words;
  fprintf c "minor_collections: %d\n" st.minor_collections;
  fprintf c "major_collections: %d\n" st.major_collections;
  fprintf c "heap_words: %d\n" st.heap_words;
  fprintf c "heap_chunks: %d\n" st.heap_chunks;
  fprintf c "top_heap_words: %d\n" st.top_heap_words;
  fprintf c "live_words: %d\n" st.live_words;
  fprintf c "live_blocks: %d\n" st.live_blocks;
  fprintf c "free_words: %d\n" st.free_words;
  fprintf c "free_blocks: %d\n" st.free_blocks;
  fprintf c "largest_free: %d\n" st.largest_free;
  fprintf c "fragments: %d\n" st.fragments;
  fprintf c "compactions: %d\n" st.compactions;
;;

let allocated_bytes () =
  let (mi, pro, ma) = counters () in
  (mi +. ma -. pro) *. float_of_int (Sys.word_size / 8)
;;

external finalise : ('a -> unit) -> 'a -> unit = "caml_final_register";;
external finalise_release : unit -> unit = "caml_final_release";;


type alarm = bool ref;;
type alarm_rec = {active : alarm; f : unit -> unit};;

let rec call_alarm arec =
  if !(arec.active) then begin
    finalise call_alarm arec;
    arec.f ();
  end;
;;

let create_alarm f =
  let arec = { active = ref true; f = f } in
  finalise call_alarm arec;
  arec.active
;;

let delete_alarm a = a := false;;
