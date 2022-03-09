open Base
open Stdio

type pwd_spec = {
  min_times: int;
  max_times: int;
  ch: char;
  pwd: string;
}

let read_to_pwd_spec in_ch =
let open Option.Let_syntax in
let%bind line = Stdio.In_channel.input_line in_ch in
let components = String.split_on_chars line ~on:['-';' ';':'] in
match components with
| min_str :: max_str :: ch_str :: _ :: pwd :: [] ->
  let min_times = Int.of_string min_str in
  let max_times = Int.of_string max_str in
  let ch = Char.of_string ch_str in
  Some { min_times; max_times; ch; pwd }
| _ -> None

let is_valid_spec { min_times ; max_times ; ch ; pwd } =
  let count = String.fold pwd ~init:0 ~f:(fun count c -> if Char.(c = ch) then count + 1 else count) in
  min_times <= count && count <= max_times

let count_valid_pwds in_ch =
  let rec count_valid_pwds_aux count =
    match read_to_pwd_spec in_ch with
    | None -> count
    | Some spec ->
      if is_valid_spec spec then
        count_valid_pwds_aux (count + 1) else
        count_valid_pwds_aux count
  in
  count_valid_pwds_aux 0

let () = printf "%d\n" (count_valid_pwds Stdio.In_channel.stdin)