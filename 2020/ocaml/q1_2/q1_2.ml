open Base
open Stdio

let rec get_sums_to_n arr l r n =
  if l >= r then None
  else let al = arr.(l) and ar = arr.(r) in
  let s = al + ar in
    if s = n then Some (al, ar)
    else if s < n then get_sums_to_n arr (l + 1) r n
    else get_sums_to_n arr l (r - 1) n

let get_sums_to_2020_3 arr =
  let rec iter arr i =
    let last = Array.length arr - 1 in
    if i + 2 > last then None
    else let ai = arr.(i) in
    match get_sums_to_n arr (i + 1) last (2020 - ai) with
    | None -> iter arr (i + 1)
    | Some (l, r) -> Some (ai * l * r)
  in
  iter arr 0

let sorted_array_from_stdin () =
  let rec iter acc =
    match In_channel.(input_line stdin) with
    | None -> acc
    | Some line -> iter (Int.of_string line :: acc)
  in
  let a = Array.of_list (iter []) in
  Array.sort a ~compare:(Int.compare);
  a

let () =
  match get_sums_to_2020_3 (sorted_array_from_stdin ()) with
  | None -> ()
  | Some prod -> printf "%d\n" prod