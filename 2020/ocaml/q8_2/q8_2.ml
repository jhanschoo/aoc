open Core
exception Exit

type instruction =
| Acc of int
| Jmp of int
| Nop of int

let instr_to_str = function
  | Acc n -> "acc " ^ Int.to_string n
  | Jmp n -> "jmp " ^ Int.to_string n
  | Nop n -> "nop " ^ Int.to_string n

let parse_instructions line =
  match String.split line ~on:' ' with
  | ["acc"; nstr] -> Acc (Int.of_string nstr)
  | ["jmp"; nstr] -> Jmp (Int.of_string nstr)
  | ["nop"; nstr] -> Nop (Int.of_string nstr)
  | _ -> raise Exit

let execute_with_seen instructions (seen, accumulator, index) =
  if Set.mem seen index
    then None
    else
      match instructions.(index) with
      | Acc n -> Some (Set.add seen index, accumulator + n, index + 1)
      | Jmp n -> Some (Set.add seen index, accumulator, index + n)
      | Nop _ -> Some (Set.add seen index, accumulator, index + 1)

let rec execute_till_seen instructions ~state =
  match execute_with_seen instructions state with
  | Some ((_,accum,index) as state) ->
    let l = Array.length instructions in
    if index >= l
      then if l = index
        then
          Some accum
        else None
      else
        execute_till_seen instructions ~state
  | None -> None

let toggle_instruction instructions index =
  let modified_instruction =
    match instructions.(index) with
    | Jmp n -> Nop n
    | Nop n -> Jmp n
    | _ -> raise Exit
  in
  instructions.(index) <- modified_instruction

let diagnose_instructions instructions = Array.iter instructions ~f:(fun instr -> printf "%s\n" (instr_to_str instr));;
diagnose_instructions;;

let execute_with_single_flip (indexes, instructions) =
  List.find_map indexes ~f:(fun index ->
    let () = toggle_instruction instructions index in
    let result = execute_till_seen instructions ~state:(Set.empty (module Int), 0, 0) in
    let () = toggle_instruction instructions index in
    result
  )

let extract_jmp_nop_indices instrs =
  instrs
  |> List.foldi ~init:[] ~f:(fun ind acc instr -> match instr with Acc _ -> acc | _ -> ind :: acc)
  |> List.rev

let () = Stdio.In_channel.input_lines Stdio.stdin
  |> List.map ~f:parse_instructions
  |> (fun l -> (extract_jmp_nop_indices l, Array.of_list l))
  |> execute_with_single_flip
  |> Option.(fun result -> result >>| printf "%d\n")
  |> Base.Fn.const ()