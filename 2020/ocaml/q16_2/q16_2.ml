open Core
(* exception Exit;; *)

module Int_string_tuple = struct
  include Tuple.Make (Int) (String)
  include Tuple.Comparable (Int) (String)
end

let rules_regex = Re.Perl.compile_pat "^(.*): (\\d*)-(\\d*) or (\\d*)-(\\d*)$"

let parse_rules =
  let f m line =
    let open Result.Let_syntax in
    let%bind groups = Result.of_option ~error:"failed matching" (Re.exec_opt rules_regex line) in
    let%bind key = Result.of_option ~error:"failed extracting key" (Re.Group.get_opt groups 1) in
    let%bind endpoints =
      List.map [2; 3; 4; 5] ~f:(Re.Group.get_opt groups)
      |> List.mapi ~f:(fun i s -> Result.of_option s ~error:("failed extracting group" ^ (Int.to_string i)))
      |> Result.all
    in
    let%bind endpoints = endpoints
      |> List.map ~f:(fun s -> Result.try_with (fun () -> Int.of_string s))
      |> List.mapi ~f:(fun i s -> Result.map_error s ~f:(Fn.const ("failed converting group" ^ (Int.to_string i))))
      |> Result.all
    in
    Ok (Map.set m ~key ~data:endpoints)
  in
  List.fold_result ~init:(Map.empty (module String)) ~f

let parse_ticket ticket =
  ticket
  |> String.split ~on:','
  |> List.map ~f:(fun s -> Result.try_with (fun () -> Int.of_string s))
  |> List.mapi ~f:(fun i s -> Result.map_error s ~f:(Fn.const ("failed converting field" ^ (Int.to_string i))))
  |> Result.all

let parse_tickets tickets =
  tickets
  |> List.map ~f:parse_ticket
  |> Result.all

let rule_validates_field field_value ~rule =
  match rule with
  | [r1_1; r1_2; r2_1; r2_2] -> (field_value >= r1_1 && field_value <= r1_2) || (field_value >= r2_1 && field_value <= r2_2)
  | _ -> false

let valid_field field ~rules = List.exists (Map.data rules) ~f:(fun rule -> rule_validates_field field ~rule)

let valid_ticket ticket ~rules = List.for_all ticket ~f:(fun field -> valid_field field ~rules)

let positions_for_rule accum rule ~values_by_field_pos ~key =
  let f i field_values = if List.for_all field_values ~f:(fun value -> rule_validates_field value ~rule) then Some i else None in
  let rules_valid_pos = List.filter_mapi values_by_field_pos ~f in
  let rules_valid_pos = Set.of_list (module Int) rules_valid_pos in
  Map.set accum ~key:(Set.length rules_valid_pos, key) ~data:rules_valid_pos

let rec resolve_rules_valid_pos rules_valid_pos ~known =
  (match Map.min_elt rules_valid_pos with
  | None -> known
  | Some ((count, key) as aug_key, positions) ->
    (match count, positions with
    | 1, s ->
      (match Set.choose s with
      | None -> known
      | Some pos ->
        let rules_valid_pos = Map.remove rules_valid_pos aug_key in
        let f ~key:(_, key) ~data a =
          let data = Set.remove data pos in
          Map.set a ~key:(Set.length data, key) ~data
        in
        let rules_valid_pos = Map.fold rules_valid_pos ~init:(Map.empty (module Int_string_tuple)) ~f in
        let known = Map.set known ~key ~data:pos in
        resolve_rules_valid_pos rules_valid_pos ~known
      )
    | _, _ -> known
    )
  )

let obtain_answer your_ticket ~known =
  let f ~key ~data product =
    (match String.is_prefix key ~prefix:"departure " with
    | true -> product * your_ticket.(data)
    | false -> product
    )
  in
  Map.fold known ~init:1 ~f

let process_with_result res =
  let open Result.Let_syntax in
  let%bind rules, your_ticket, nearby_tickets = match res with
  | nearby_tickets :: your_ticket :: rules :: _ -> Ok (rules, your_ticket, nearby_tickets)
  | _ -> Error "process_with_result: unexpected result"
  in
  let%bind rules = parse_rules rules and your_ticket = parse_tickets your_ticket and nearby_tickets = parse_tickets nearby_tickets in
  let%bind your_ticket = (match your_ticket with | [ticket] -> Ok (Array.of_list ticket) | _ -> Error "your_ticket does not contain single ticket") in
  let nearby_tickets = List.filter nearby_tickets ~f:(fun ticket -> valid_ticket ticket ~rules) in
  let%bind values_by_field_pos = Result.of_option (List.transpose nearby_tickets) ~error:"invalid transposition" in
  let rules_valid_pos = Map.fold rules ~init:(Map.empty (module Int_string_tuple)) ~f:(fun ~key ~data:rule accum -> positions_for_rule accum rule ~values_by_field_pos ~key) in
  let known = resolve_rules_valid_pos rules_valid_pos ~known:(Map.empty (module String)) in
  let answer = obtain_answer your_ticket ~known in
  Ok (printf "Answer: %d\n" answer)

let () =
  let res = Stdio.In_channel.input_lines Stdio.stdin in
  let res, _ =
    let f (groups, prev) v =
      if String.(prev = "") then ([] :: groups, v)
      else
      if String.(v = "") then (groups, v) else
      match groups with
      | last_group :: rest -> ((v :: last_group) :: rest, v)
      | _ -> ([[v]], v) in
    List.fold res ~init:([], "dummy") ~f in
  process_with_result res
  |> Fn.const ()