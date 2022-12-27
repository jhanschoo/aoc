open Core

let () =
  let res = Stdio.In_channel.input_lines Stdio.stdin in
  let numbers = Array.of_list_map res ~f:Int.of_string in
  let () = Array.sort numbers ~compare:Int.compare in
  let f = function None -> 1 | Some v -> v + 1 in
  let f (results, prev) v = (Map.update results (v - prev) ~f, v) in
  let results, _ = Array.fold numbers ~init:(Map.singleton (module Int) 3 1, 0) ~f in
  let one_jolt = match Map.find results 1 with None -> 0 | Some n -> n and
    three_jolt = match Map.find results 3 with None -> 0 | Some n -> n in
  printf "%d\n" (one_jolt * three_jolt)