open Core;;
exception Exit;;

(* let result = 127 *)
let result = 15690279

let rec process_list total incl excl =
  if total = result
  then Some (incl, excl)
  else
  if total < result
  then match excl with
  | [] -> None
  | n :: rest -> process_list (total + n) incl rest
  else match incl with
  | [] -> None
  | n :: rest -> process_list (total - n) rest excl

let rec extract_min_max min max incl excl =
  if Poly.(incl = excl)
  then (min, max)
  else
    match incl with
  | [] -> raise Exit
  | n :: rest -> extract_min_max (Int.min min n) (Int.max max n) rest excl

let () =
  let lines = Stdio.In_channel.input_lines Stdio.stdin in
  let numbers = List.map lines ~f:Int.of_string in
  match process_list 0 numbers numbers with
  | Some (n :: incl, excl) ->
    let (min, max) = extract_min_max n n incl excl in
    printf "%d\n" (min + max)
  | _ -> raise Exit