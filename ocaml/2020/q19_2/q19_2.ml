open Core
exception Exit

type rule =
  | RExpr of int list list
  | RChar of char
  | RFinal
  [@@deriving sexp]

let parse_rule line ~rules =
  let i, expr = String.lsplit2_exn line ~on:':' in
  let i = Int.of_string i in
  let rule =
    if Int.(i = 8)
      then RExpr [[42]; [42; 8]]
    else if Int.(i = 11)
      then RExpr [[42; 31]; [42; 11; 31]]
    else if String.(suffix expr 1 = "\"")
      then RChar (String.get expr (String.length expr - 2))
    else
      RExpr (
        expr
        |> String.split ~on:'|'
        |> List.map ~f:String.strip
        |> List.map ~f:(String.split ~on:' ')
        |> List.map ~f:(List.map ~f:Int.of_string)
      )
  in
  Map.set rules ~key:i ~data:rule

let rec parse_rules lines ~rules =
  (match lines with
  | [] -> raise Exit
  | "" :: rest -> Map.set rules ~key:(-1) ~data:RFinal, rest
  | line :: rest -> parse_rules rest ~rules:(parse_rule line ~rules)
  )

let rec eval_one_or c ~ors ~rules =
  List.concat_map ors ~f:(eval_one_state c ~rules)
and eval_one_state c st ~rules =
  (match st with
  | i :: rest ->
    (match Map.find_exn rules i with
    | RChar t ->
      if Char.(c = t) then [rest]
      else []
    | RExpr expansions ->
      let f clause = List.concat [clause; rest] in
      eval_one_or c ~ors:(List.map expansions ~f) ~rules
    | RFinal -> []
    )
  | [] -> []
  )

let sat s ~rules =
  let f ors c = eval_one_or c ~ors ~rules in
  let finals = String.fold s ~init:[[0;-1]] ~f in
  List.mem finals [-1] ~equal:(List.equal Int.equal)

let () =
  let res = Stdio.In_channel.input_lines Stdio.stdin in
  let rules, rest = parse_rules res ~rules:(Map.empty (module Int)) in
  let sat_counts = List.count rest ~f:(sat ~rules) in
  printf "%d\n" sat_counts