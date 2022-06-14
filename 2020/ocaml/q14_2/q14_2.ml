open Core

type bitmask = { or_mask: Int64.t; fluct_indexes: int list } [@@deriving sexp]

type write = Int64.t * int [@@deriving sexp]

let zero_bitmask = { or_mask = Int64.zero; fluct_indexes = [] }

type instr =
  | Bitmask of bitmask
  | Write of write
  [@@deriving sexp]

let app_0 i64 = Int64.(i64 lsl 1)
let app_1 i64 = Int64.(i64 lsl 1 + Int64.one)

let set_0_at_i i64 i = Int64.(i64 land (lnot (Int64.one lsl i)))
let set_1_at_i i64 i = Int64.(i64 lor (Int64.one lsl i))

let rec parse_bitmask ({ or_mask; fluct_indexes } as mask) ind arr =
  if ind >= Array.length arr then mask
  else
  (match arr.(ind) with
  | '1' ->
    let or_mask = app_1 or_mask in
    parse_bitmask { or_mask; fluct_indexes } (ind + 1) arr
  | '0' ->
    let or_mask = app_0 or_mask in
    parse_bitmask { or_mask; fluct_indexes } (ind + 1) arr
  | _ ->
    let rev_i = Array.length arr - ind - 1 in
    let or_mask = app_0 or_mask
    and fluct_indexes = rev_i :: fluct_indexes in
    parse_bitmask { or_mask; fluct_indexes } (ind + 1) arr
  )

let evaluate_write (addr, value) ~accum:(state, { or_mask; fluct_indexes }) =
  let open Int64 in
  let addr = addr lor or_mask in
  let f addrs ind =
    let f res_addrs addr = set_0_at_i addr ind :: set_1_at_i addr ind :: res_addrs in
    List.fold addrs ~init:[] ~f
  in
  let addrs = List.fold fluct_indexes ~init:[addr] ~f in
  let f state addr = Map.set state ~key:addr ~data:value in
  List.fold addrs ~init:state ~f

let evaluate_program program =
  let f ((state, mask) as accum) = (function
  | Bitmask mask -> state, mask
  | Write write -> evaluate_write write ~accum, mask
  ) in
  let state, _ = List.fold program ~init:(Map.empty (module Int64), zero_bitmask) ~f in
  state

let parse_line line =
  (match String.chop_prefix line ~prefix:"mask = " with
  | Some m -> Bitmask (parse_bitmask zero_bitmask 0 (String.to_array m))
  | None ->
    let addr, value = String.lsplit2_exn line ~on:'=' in
    let addr = Int64.of_string (String.strip ~drop:(function 'm' | 'e' | '[' | ']' | ' ' -> true | _ -> false) addr) in
    let value = Int.of_string (String.strip value) in
    Write (addr, value)
  )

let () =
  let res = Stdio.In_channel.input_lines Stdio.stdin in
  let program = List.map res ~f:parse_line in
  let state = evaluate_program program in
  let s = Map.fold state ~init:0 ~f:(fun ~key:_ ~data s -> s + data) in
  printf "%d\n" s