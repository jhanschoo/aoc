open Core

(* note that there is only one adapter to each joltage rating, which simplifies things *)

let rec count_arrangements i prev ~adapters ~memo =
  let curr = adapters.(i) in
  if (prev + 3 < curr) then (0, memo)
  else
  if i = Array.length adapters - 1 then (1, memo)
  else
  let count_others, memo = count_arrangements (i + 1) prev ~adapters ~memo in
  let count_self, memo =
    (match Map.find memo i with
    | Some result -> (result, memo)
    | None -> count_arrangements (i + 1) curr ~adapters ~memo
    )
  in
  let result = count_others + count_self in
  result, Map.set memo ~key:i ~data:count_self

let () =
  let res = Stdio.In_channel.input_lines Stdio.stdin in
  let adapters = Array.of_list_map res ~f:Int.of_string in
  let () = Array.sort adapters ~compare:Int.compare in
  let count, _ = count_arrangements 0 0 ~adapters ~memo:(Map.empty (module Int)) in
  printf "%d\n" count