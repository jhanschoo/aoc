open Core
(* exception Exit;; *)

let rules_regex = Re.Perl.compile_pat "^(.*): (\\d*)-(\\d*) or (\\d*)-(\\d*)$"

let process_rules =
  let f m line =
    let open Result.Let_syntax in
    let%bind groups = Result.of_option ~error:"failed matching" (Re.exec_opt rules_regex line) in
    let%bind key = Result.of_option ~error:"failed extracting key" (Re.Group.get_opt groups 1) in
    let%bind r1_1 = Result.of_option ~error:"failed extracting r1_1" (Re.Group.get_opt groups 2) in
    let%bind r1_2 = Result.of_option ~error:"failed extracting r1_2" (Re.Group.get_opt groups 3) in
    let%bind r2_1 = Result.of_option ~error:"failed extracting r2_1" (Re.Group.get_opt groups 4) in
    let%bind r2_2 = Result.of_option ~error:"failed extracting r2_2" (Re.Group.get_opt groups 5) in
    Ok (Map.set m ~key ~data:(r1_1, r1_2))
    (* Ok (Map.set m ~key ~data:((r1_1, r1_2), (r2_1, r2_2)))  *)
  in
  List.fold_result ~init:(Map.empty (module String)) ~f

let a = (module (struct include Tuple.Make (String) (String) include Tuple.Sexpable (String) (String) end) : Sexpable.S);;

let process_tickets tickets = Ok tickets

let process_with_result res =
  let open Result.Let_syntax in
  let%bind rules, your_ticket, nearby_tickets = match res with
  | nearby_tickets :: your_ticket :: rules :: _ -> Ok (rules, your_ticket, nearby_tickets)
  | _ -> Error "process_with_result: unexpected result"
  in
  let%bind rules = process_rules rules and your_ticket = process_tickets your_ticket and nearby_tickets = process_tickets nearby_tickets in
  let () = printf "%s" (String.t_of_sexp (Map.sexp_of_m__t (module String) (module (Tuple.Sexpable (module String) (module String))) rules)) in
  Ok (rules, your_ticket, nearby_tickets)

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