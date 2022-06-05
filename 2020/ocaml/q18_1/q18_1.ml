open Core;;
(* exception Exit;; *)

type operator = Multiply | Add

type expression =
  | Value of int
  | Open_parenthesis
  | Close_parenthesis
  | Operator of operator

let parse line = line
  |> String.concat_map ~f:(function '(' -> "( " | c -> String.of_char c)
  |> String.concat_map ~f:(function ')' -> " )" | c -> String.of_char c)
  |> String.split ~on:' '
  |> List.map ~f:(function
    | "(" -> Open_parenthesis
    | ")" -> Close_parenthesis
    | "*" -> Operator Multiply
    | "+" -> Operator Add
    | s -> Value (Int.of_string s))

let rec evaluate instrs ~stack =
  match instrs, stack with
  (* at end of input, stack should contain a sole value; we return it *)
  | [], [Value n] -> Ok n
  (* at intermediate or end of a clause *)
  | Value vr :: instrs, Operator o :: Value vl :: stack ->
    let op = match o with Multiply -> Int.( * ) | Add -> Int.( + ) in
    evaluate instrs ~stack:(Value (op vl vr) :: stack)
  | Close_parenthesis :: instrs, Value v :: Open_parenthesis :: stack ->
    evaluate (Value v :: instrs) ~stack
  | Close_parenthesis :: _, _ -> Error "unexpected close parenthesis"
  | (Value _ as s) :: instrs, stack
  | (Operator _ as s) :: instrs, stack
  | (Open_parenthesis as s) :: instrs, stack -> evaluate instrs ~stack:(s :: stack)
  | [], _ -> Error "unexpected end of input"

let () =
  let numbers =
  Stdio.In_channel.input_lines Stdio.stdin
    |> List.map ~f:parse
    |> List.map ~f:(evaluate ~stack:[])
    |> Result.all
  in
  (match numbers with
    | Ok numbers -> printf "%d\n" (List.sum (module Int) numbers ~f:Fn.id)
    | Error s -> printf "%s\n" s
  )