open Core
exception Exit

let list_to_set (l : (int * (int * int)) list) = Set.of_list (module Tuple.Comparator (Int) (Tuple.Comparator (Int) (Int))) l

let process_map line = List.foldi line ~init:[] ~f:(fun i l r -> String.foldi r ~init:l ~f:(fun j l c -> if Char.(c = '#') then (i,j,0) :: l else l))

let () = Stdio.In_channel.input_lines Stdio.stdin
  |> process_map
  |> Set.of_list (module (Tuple (module Int)))
  |> printf "%d\n"