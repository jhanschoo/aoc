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
  let update_map map (color, count) = Map.update map color ~f:(function Some n -> n + count | None -> count) in
  match Map.min_elt nonterminals with
  | Some (color, count) ->
    let nonterminals_without_key = Map.remove nonterminals color in
    let next_terminals = update_map terminals (color, count) in
    let next_nonterminals = List.fold (Map.find_exn table color) ~init:nonterminals_without_key ~f:update_map in
    process_rules table next_terminals next_nonterminals
  | None -> terminals

let terminals_of_color table color =
  process_rules table (Map.empty (module String)) (Map.singleton (module String) color 1)

let contains_gold table color =
  let terminals = terminals_of_color table color
in Map.mem terminals "shiny gold"

let () = Stdio.In_channel.input_lines Stdio.stdin
  |> List.map ~f:parse_rules
  |> (fun l -> (List.map l ~f:(fun (a, _) -> a), Map.of_alist_exn (module String) l))
  |> (fun (colors, table) -> List.filter colors ~f:(contains_gold table))
  |> List.length
  (* remove the count of the gold bag itself *)
  |> (fun n -> n - 1)
  |> printf "%d\n"