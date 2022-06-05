open Core;;
(* exception Exit;; *)

type operator = Multiply | Add [@@deriving sexp]

type expression =
  | Value of int
  | Open_parenthesis
  | Close_parenthesis
  | Operator of operator
  [@@deriving sexp]

let parse line = line
  |> String.concat_map ~f:(function '(' -> "( " | c -> String.of_char c)
  |> String.concat_map ~f:(function ')' -> " )" | c -> String.of_char c)
  |> String.split ~on:' '
  |> List.map ~f:(function
    | "(" -> Open_parenthesis
    | ")" -> Close_parenthesis
    | "*" -> Operator Multiply
    | "+" -> Operator Add
    | s -> Value (Int.of_string s)
    )

let rec to_postfix instrs ~stack ~output =
  (match instrs, stack with
  | (Value _ as instr) :: instrs, _ -> to_postfix instrs ~stack ~output:(instr :: output)
  | Open_parenthesis :: instrs, _ -> to_postfix instrs ~stack:(Open_parenthesis :: stack) ~output
  | Close_parenthesis :: instrs, Open_parenthesis :: stack -> to_postfix instrs ~stack ~output
  | Close_parenthesis :: _, v :: stack -> to_postfix instrs ~stack ~output:(v :: output)
  | Close_parenthesis :: _, _ -> Error ("unbalanced parentheses", [instrs; stack; output])
  | (Operator Add as op) :: instrs, Operator Multiply :: _
  | (Operator _ as op) :: instrs, [] -> to_postfix instrs ~stack:(op :: stack) ~output
  | (Operator _ as op) :: instrs, Open_parenthesis :: _ -> to_postfix instrs ~stack:(op :: stack) ~output
  | (Operator _  :: _ as instrs), (Operator _ as op2) :: stack -> to_postfix instrs ~stack ~output:(op2 :: output)
  | Operator _ :: _, _ -> Error ("unexpected operator", [instrs; stack; output])
  | [], v :: stack -> to_postfix [] ~stack ~output:(v :: output)
  | [], [] -> Ok (List.rev output)
  )


let rec evaluate instrs ~stack =
  (match instrs, stack with
  (* at end of input, stack should contain a sole value; we return it *)
  | [], [Value n] -> Ok n
  | Value v :: instrs, _ -> evaluate instrs ~stack:(Value v :: stack)
  | Operator Add :: instrs, Value v1 :: Value v2 :: stack -> evaluate instrs ~stack:(Value (v1 + v2) :: stack)
  | Operator Multiply :: instrs, Value v1 :: Value v2 :: stack -> evaluate instrs ~stack:(Value (v1 * v2) :: stack)
  | Operator _ :: instrs, _ -> Error ("unexpected operator", [instrs; stack])
  | Open_parenthesis :: _, _ | Close_parenthesis :: _, _ -> Error ("unexpected parentheses", [instrs; stack])
  | [], _ -> Error ("unexpected end of input", [instrs; stack])
  )

let () =
  let result =
    let open Result.Let_syntax in
    let%bind numbers =
    Stdio.In_channel.input_lines Stdio.stdin
      |> List.map ~f:parse
      |> List.map ~f:(to_postfix ~stack:[] ~output:[])
      |> Result.all
    in
    List.map numbers ~f:(evaluate ~stack:[])
    |> Result.all
  in
  (match result with
  | Ok expressions -> printf "%d\n" (List.sum (module Int) expressions ~f:Fn.id)
  | Error (s, state) -> printf "%s\n" (s ^ (Sexp.to_string_hum (List.sexp_of_t (List.sexp_of_t sexp_of_expression) state)))
  )