open Core
exception Empty_list

let rec parse_rules_words_contained words accum =
  match words with
  | count::property::color::_::rest -> parse_rules_words_contained rest ((property ^ " " ^ color,Int.of_string count)::accum)
  | _ -> accum

let parse_rules_words = function
  | property::color::_::_::rest ->
    let containeds = parse_rules_words_contained rest []
    in
      (property ^ " " ^ color,containeds)
  | _ -> raise Empty_list

let parse_rules line =
  line
  |> String.split ~on:' '
  |> parse_rules_words

let rec process_rules table terminals nonterminals =
  match Map.min_elt nonterminals with
  | Some (container, container_count) ->
    let nonterminals_without_key = Map.remove nonterminals container in
    let next_terminals = Map.update terminals container ~f:(function Some n -> n + container_count | None -> container_count) in
    let f prev_nonterminals (contained, contained_count) = Map.update prev_nonterminals contained ~f:(function Some n -> n + contained_count * container_count | None -> contained_count * container_count) in
    let next_nonterminals = List.fold (Map.find_exn table container) ~init:nonterminals_without_key ~f in
    process_rules table next_terminals next_nonterminals
  | None -> terminals

let terminals_of_container table container =
  process_rules table (Map.empty (module String)) (Map.singleton (module String) container 1)

let () = Stdio.In_channel.input_lines Stdio.stdin
  |> List.map ~f:parse_rules
  |> Map.of_alist_exn (module String)
  |> (fun table -> terminals_of_container table "shiny gold")
  (* |> (fun m -> let () = Map.iteri m ~f:(fun ~key ~data -> printf "%s: %d\n" key data) in m) *)
  |> Map.fold ~init:0 ~f:(fun ~key:_ ~data:count accum -> count + accum)
  |> (fun n -> n - 1)
  |> printf "%d\n"