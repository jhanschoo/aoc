open Base
open Stdio

let is_tree ch = Char.(ch = '#')

let move (x, strs) right down =
  match List.drop strs down with
  | [] -> None
  | (str :: _ as strs_next) -> Some ((x + right) % (String.length str), strs_next)

let trees_at x str =
  match is_tree (String.get str x) with
  | true -> 1
  | false -> 0

let rec add_trees_from strs x right down count =
  match move (x, strs) right down with
  | None | Some (_, []) -> count
  | Some (x_next, (str :: _ as strs_next)) ->
    let count_next = count + trees_at x_next str
    in add_trees_from strs_next x_next right down count_next

let () =
  let strs = Stdio.In_channel.input_lines Stdio.stdin
  and params = [(1,1);(3,1);(5,1);(7,1);(1,2)] in
  let f prod (right,down) = prod * add_trees_from strs 0 right down 0 in
  printf "%d\n" (List.fold params ~init:1 ~f)