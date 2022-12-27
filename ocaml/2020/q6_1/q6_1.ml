open Core
exception Empty_list

let lines_to_form_yeses (lines : string list) =
  let f stanzas line =
    match line with
    | "" -> [] :: stanzas
    | s -> match stanzas with
            | [] -> raise Empty_list
            | (stanza :: rest) ->
              (s :: stanza) :: rest
  in
  lines
  |> List.fold ~init:[[]] ~f
  |> List.map ~f:String.concat

let yes_strings_to_sets yes_string = Set.of_list (module Char) (String.to_list yes_string)

let () = Stdio.In_channel.input_lines Stdio.stdin
  |> lines_to_form_yeses
  |> List.map ~f:yes_strings_to_sets
  |> List.map ~f:(Set.length)
  |> List.sum (module Int) ~f:Base.Fn.id
  |> printf "%d\n"