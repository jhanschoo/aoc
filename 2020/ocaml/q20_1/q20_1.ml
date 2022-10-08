open Core
exception Illegal_input of string list
exception Illegal_argument

let parse_info info = Int.of_string (String.drop_prefix (String.drop_suffix info 1) 5)

let hash_border =
  let f prev = function
    | '.' -> prev * 2
    | '#' -> prev * 2 + 1
    | _ -> raise Illegal_argument
  in
  List.fold ~init:0 ~f

let register_border_hash m hash tile_id =
  Map.update m hash ~f:(function Some l -> tile_id::l | None -> [tile_id])

let register_border m border tile_id =
  register_border_hash m (hash_border (List.rev border)) tile_id

let register_tile m (tile_id, tile) =
  let m = register_border m (String.to_list tile.(0)) tile_id in
  let m = register_border m (String.to_list_rev tile.(0)) tile_id in
  let m = register_border m (String.to_list tile.(9)) tile_id in
  let m = register_border m (String.to_list_rev tile.(9)) tile_id in
  let right = Array.fold tile ~init:[] ~f:(fun acc s -> String.get s 9 :: acc) in
  let m = register_border m right tile_id in
  let m = register_border m (List.rev right) tile_id in
  let left = Array.fold tile ~init:[] ~f:(fun acc s -> String.get s 0 :: acc) in
  let m = register_border m left tile_id in
  let m = register_border m (List.rev left) tile_id in
  m

let rec parse_lines = function
  | "" :: rest -> parse_lines rest
  | info :: l1 :: l2 :: l3 :: l4 :: l5 :: l6 :: l7 :: l8 :: l9 :: l10 :: rest ->
    let tile = Array.of_list [l1; l2; l3; l4; l5; l6; l7; l8; l9; l10] in
    (parse_info info, tile) :: parse_lines rest
  | [] -> []
  | (_ as input) -> raise (Illegal_input input)

let () =
  let lines = Stdio.In_channel.input_lines Stdio.stdin in
  let tiles = parse_lines lines in
  let m = Map.empty (module Int) in
  let m = List.fold ~init:m ~f:register_tile tiles in
  let f ~key:_ ~data rev_m =
    (match data with
      | [tile_id] -> Map.update rev_m tile_id ~f:(function Some i -> i + 1 | None -> 1)
      | _ -> rev_m
    ) in
  let rev_m = Map.fold m ~init:(Map.empty (module Int)) ~f in
  let rev_m = Map.filter rev_m ~f:(fun v -> v = 4) in
  let total = Map.fold rev_m ~init:1 ~f:(fun ~key ~data:_ acc -> acc * key) in
  printf "%d\n" total
  (* Map.iteri rev_m ~f:(fun ~key ~data -> printf "%d %d\n" key data) *)