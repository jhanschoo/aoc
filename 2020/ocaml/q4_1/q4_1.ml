open Base
open Stdio
exception Empty_list

let lines_to_passport_fields lines =
  let f passports line =
    match line with
    | "" -> [] :: passports
    | s -> match passports with
            | [] -> raise Empty_list
            | (passport :: rest) ->
              (s :: passport) :: rest
  in
  lines
  |> List.fold ~init:[[]] ~f
  |> List.map ~f:(String.concat ~sep:" ")
  |> List.map ~f:(String.split ~on:' ')

let filter_cid passport =
  passport
  |> List.filter ~f:(fun field -> not (String.is_prefix ~prefix:"cid:" field))

let () = Stdio.In_channel.input_lines Stdio.stdin
    |> lines_to_passport_fields
    |> List.map ~f:(filter_cid)
    |> List.filter ~f:(fun passport -> List.length passport >= 7)
    |> List.length
    |> printf "%d\n"
