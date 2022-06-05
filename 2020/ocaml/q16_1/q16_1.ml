open Core
(* exception Exit;; *)

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

let rule_validats_field field ~rule =
  match rule with
  | [r1_1; r1_2; r2_1; r2_2] -> field >= r1_1 && field <= r1_2 || field >= r2_1 && field <= r2_2
  | _ -> false

let valid_field field ~rules = List.exists (Map.data rules) ~f:(fun rule -> rule_validats_field field ~rule)

(* let valid_ticket ticket ~rules = List.exists ticket ~f:(fun field -> valid_field field ~rules) *)

let invalid_fields_of_ticket ticket ~rules = List.filter ticket ~f:(fun field -> not (valid_field field ~rules))

let process_with_result res =
  let open Result.Let_syntax in
  let%bind rules, your_ticket, nearby_tickets = match res with
  | nearby_tickets :: your_ticket :: rules :: _ -> Ok (rules, your_ticket, nearby_tickets)
  | _ -> Error "process_with_result: unexpected result"
  in
  let%bind rules = parse_rules rules and _ = parse_tickets your_ticket and nearby_tickets = parse_tickets nearby_tickets in
  (* let () = printf "%s" (Sexp.to_string (Map.sexp_of_m__t (module String) (List.sexp_of_t (Int.sexp_of_t)) rules)) in *)
  let invalid_fields_by_ticket = List.map nearby_tickets ~f:(invalid_fields_of_ticket ~rules) in
  let sum = List.sum (module Int) ~f:Fn.id (List.map invalid_fields_by_ticket ~f:(List.sum (module Int) ~f:Fn.id)) in
  Ok (printf "%d\n" sum)

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