open Core

type bitmask = { or_mask: Int64.t; and_mask: Int64.t } [@@deriving sexp]

let zero_bitmask = { or_mask = Int64.zero; and_mask = Int64.zero }

type instr =
  | Bitmask of bitmask
  | Write of int * Int64.t
  [@@deriving sexp]
let app_0 i64 = Int64.(i64 lsl 1)
let app_1 i64 = Int64.(i64 lsl 1 + Int64.one)

let rec parse_bitmask ({ or_mask; and_mask } as mask) =
  (function
  | [] -> mask
  | '1' :: rest ->
    let or_mask = app_1 or_mask
    and and_mask = app_1 and_mask in
    parse_bitmask { or_mask; and_mask } rest
  | '0' :: rest ->
    let or_mask = app_0 or_mask
    and and_mask = app_0 and_mask in
    parse_bitmask { or_mask; and_mask } rest
  | _ :: rest ->
    let or_mask = app_0 or_mask
    and and_mask = app_1 and_mask in
    parse_bitmask { or_mask; and_mask } rest
  )

let parse_line line =
  (match String.chop_prefix line ~prefix:"mask = " with
  | Some m -> Bitmask (parse_bitmask zero_bitmask (String.to_list m))
  | None ->
    let addr, value = String.lsplit2_exn line ~on:'=' in
    let addr = Int.of_string (String.strip ~drop:(function 'm' | 'e' | '[' | ']' | ' ' -> true | _ -> false) addr) in
    let value = Int64.of_string (String.strip value) in
    Write (addr, value)
  )

let evaluate_program program =
  let f (state, ({ and_mask; or_mask } as mask)) = (function
  | Bitmask mask -> state, mask
  | Write (addr, value) -> Map.set state ~key:addr ~data:Int64.(value land and_mask lor or_mask), mask
  ) in
  let state, _ = List.fold program ~init:(Map.empty (module Int), zero_bitmask) ~f in
  state

let () =
  let res = Stdio.In_channel.input_lines Stdio.stdin in
  let program = List.map res ~f:parse_line in
  let state = evaluate_program program in
  let s = Map.fold state ~init:Int64.zero ~f:(fun ~key:_ ~data s -> Int64.(s + data)) in
  printf "%d\n" (Int.of_int64_exn s)