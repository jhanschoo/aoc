open Base
open Stdio

let rec acc_inv_2020 next_int set =
  match next_int () with
  | None -> None
  | Some x ->
    let x_cpl = 2020 - x in
    if Set.mem set x then
      Some (x * x_cpl)
    else
      acc_inv_2020 next_int (Set.add set x_cpl)

let next_int_from_stdin () =
  match In_channel.input_line In_channel.stdin with
  | None -> None
  | Some l -> Some (Int.of_string l)

let () =
  match acc_inv_2020 next_int_from_stdin (Set.empty (module Int)) with
  | None -> ()
  | Some prod -> printf "%d\n" prod