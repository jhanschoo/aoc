open Core
exception Exit

type instruction =
| Acc of int
| Jmp of int
| Nop

let parse_instructions line =
  match String.split line ~on:' ' with
  | ["acc"; nstr] -> Acc (Int.of_string nstr)
  | ["jmp"; nstr] -> Jmp (Int.of_string nstr)
  | ["nop"; _] -> Nop
  | _ -> raise Exit

let execute_with_seen instructions (seen, accumulator, index) =
  match Set.mem seen index with
  | true -> None
  | _ ->
    match instructions.(index) with
    | Acc n -> Some (Set.add seen index, accumulator + n, index + 1)
    | Jmp n -> Some (Set.add seen index, accumulator, index + n)
    | Nop -> Some (Set.add seen index, accumulator, index + 1)

let rec execute_till_seen instructions ~state =
  match execute_with_seen instructions state with
  | Some state -> execute_till_seen instructions ~state
  (* | Some (_, accum, _) -> accum *)
  | None ->
    match state with
    | (_, accum, _) -> accum

let () = Stdio.In_channel.input_lines Stdio.stdin
  |> List.map ~f:parse_instructions
  |> Array.of_list
  |> execute_till_seen ~state:(Set.empty (module Int), 0, 0)
  |> printf "%d\n"