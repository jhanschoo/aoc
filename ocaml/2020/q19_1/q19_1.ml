open Core

type rule =
  | RExpr of int list list
  | RChar of char
  [@@deriving sexp]

let parse_rule line ~rules =
  let i, expr = String.lsplit2_exn line ~on:':' in
  let i = Int.of_string i in
  let rule =
    if String.(suffix expr 1 = "\"")
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
  | [] -> rules, lines
  | "" :: rest -> rules, rest
  | line :: rest -> parse_rules rest ~rules:(parse_rule line ~rules)
  )

let emp = Set.empty (module Int)

let rec eval_nfa s ~ind ~rind ~len ~rules =
  if ind >= len then emp else
  (match Map.find rules rind with
  | None -> emp
  | Some (RChar c) ->
    if Char.(String.get s ind = c) then Set.singleton (module Int) (ind + 1)
    else emp
  | Some (RExpr e) ->
    let f accum ind ~rind = Set.union (eval_nfa s ~ind ~rind ~len ~rules) accum in
    let f inds rind = Set.fold inds ~init:(Set.empty (module Int)) ~f:(f ~rind) in
    let f conj = List.fold conj ~init:(Set.singleton (module Int) ind) ~f in
    Set.union_list (module Int) (List.map e ~f)
  )

let sat s ~rules =
  let len = String.length s in
  let final = eval_nfa s ~ind:0 ~rind:0 ~len ~rules in
  Set.mem final len


let () =
  let res = Stdio.In_channel.input_lines Stdio.stdin in
  let rules, rest = parse_rules res ~rules:(Map.empty (module Int)) in
  let sat_counts = List.count rest ~f:(sat ~rules) in
  printf "%d\n" sat_counts