open Base
open Stdio

let is_tree ch = Char.(ch = '#')

let move_right x str = (x + 3) % (String.length str)

let trees_at x str = if is_tree (String.get str x) then 1 else 0

let rec add_trees_in strs x count =
  match strs with
  | [] -> count
  | (str::strs) ->
    add_trees_in strs (move_right x str) (count + trees_at x str)

let () = printf "%d\n" (add_trees_in (List.tl_exn (Stdio.In_channel.input_lines Stdio.stdin)) 3 0)