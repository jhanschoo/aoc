open Core
exception Illegal_input of string list
exception Illegal_argument

module Oriented_tile = struct
  module T = struct
    type t = int * D4.t [@@deriving compare, equal, sexp_of]
  end
include T
include Comparator.Make(T)
end

module Coords = struct
  module T = struct
    type t = int * int [@@deriving compare, equal, sexp_of]
  end
include T
include Comparator.Make(T)
end

let parse_info info = Int.of_string (String.drop_prefix (String.drop_suffix info 1) 5)

let hash_border =
  let f prev = function
    | '.' -> prev * 2
    | '#' -> prev * 2 + 1
    | _ -> raise Illegal_argument
  in
  List.fold ~init:0 ~f

let register_border_hash (border_map, rev_border_map) hash tile_id sigma =
  let border_map = Map.set border_map ~key:(tile_id, sigma) ~data:hash in
  let rev_border_map = Map.update rev_border_map hash ~f:(function
    | Some l -> (tile_id, sigma)::l
    | None -> [(tile_id, sigma)]
  ) in
  (border_map, rev_border_map)

let register_border border_maps border tile_id sigma =
  register_border_hash border_maps (hash_border (List.rev border)) tile_id sigma

  (* Convention: A rotates a square tau/4 anti-clockwise, B flips a square horizontally across the midpoint vertical axis *)
let register_tile ~key ~data border_maps =
  (* topleft to topright *)
  let border_maps = register_border border_maps (String.to_list data.(0)) key D4.E in
  (* topright to topleft *)
  let border_maps = register_border border_maps (String.to_list_rev data.(0)) key D4.B in
  let right = Array.fold_right data ~f:(fun s acc -> String.get s 9 :: acc) ~init:[] in
  (* topright to bottomright *)
  let border_maps = register_border border_maps right key D4.A in
  (* bottomright to topright *)
  let border_maps = register_border border_maps (List.rev right) key D4.BA in
  (* bottomright to bottomleft *)
  let border_maps = register_border border_maps (String.to_list_rev data.(9)) key D4.A2 in
  (* bottomleft to bottomright *)
  let border_maps = register_border border_maps (String.to_list data.(9)) key D4.BA2 in
  let left = Array.fold data ~init:[] ~f:(fun acc s -> String.get s 0 :: acc) in
  (* bottomleft to topleft *)
  let border_maps = register_border border_maps left key D4.A3 in
  (* topleft to bottomleft *)
  let border_maps = register_border border_maps (List.rev left) key D4.BA3 in
  border_maps

(*
  Suppose that A is the border topleft to topright of (tile_1, sigma).
  Suppose that B is the border topleft to topright of (tile_2, pi) such that (tile_2, pi) is immediately above (tile_1, sigma).
  Then A is also the border bottomleft to bottomright of (tile_2, pi).
  Then A is the border topleft to topright of (tile_2, BA2 * sigma),
  and observe that the border bottomleft to bottomright of (tile_2, BA2 * sigma)
  is the border topleft to topright of (tile_2, BA2 * BA2 * sigma = sigma);
  that is, it is B.

  Hence given (tile_1, sigma),
  the go_forward function first finds A,
  then finds (tile_2, BA2 * pi),
  then computes (tile_2, pi).

  Of course, if we replace the border topleft to topright with some other edge, due to group properties we will essentially also "go forward" with respect to that edge by executing this function, hence the name.
*)
let go_forward (border_map, rev_border_map) o_tile =
  let open Option.Let_syntax in
  let%bind hash = Map.find border_map o_tile in
  let%bind o_tiles = Map.find rev_border_map hash in
  (
    match o_tiles with
    | [o_tile_1; o_tile_2] ->
      let (next_tile_id, next_sigma) =
      if Oriented_tile.equal o_tile_1 o_tile then o_tile_2 else o_tile_1
      in
      Some (next_tile_id, D4.(cps BA2 next_sigma))
    | _ -> None
  )

let rec go_forward_trans_clos_fold border_maps o_tile ~init ~f =
  match go_forward border_maps o_tile with
  | Some next_o_tile -> go_forward_trans_clos_fold border_maps next_o_tile ~init:(f init o_tile) ~f
  | None -> o_tile, f init o_tile

let go_to_top_left border_maps o_tile =
  let (tile_id, sigma), _ = go_forward_trans_clos_fold border_maps o_tile ~init:() ~f:Fn.const in
  (* rotate map 3tau/4 anticlockwise / rotate self tau/4 anticlockwise *)
  let o_tile = (tile_id, D4.(cps A3 sigma)) in
  let (tile_id, sigma), _ = go_forward_trans_clos_fold border_maps o_tile ~init:() ~f:Fn.const in
  (* rotate map tau/4 anticlockwise / rotate self tau/4 clockwise *)
  (tile_id, D4.(cps A sigma))

let chart_row_from_left border_maps coords_map coords left_o_tile =
  let tile_id, sigma = left_o_tile in
  let coords_next (x, y) = x, y + 1 in
  let coords_map = Map.set coords_map ~key:coords ~data:left_o_tile in
  (* rotate map tau/4 anticlockwise / rotate self tau/4 clockwise *)
  let o_tile = tile_id, D4.(cps A sigma) in
  let f (coords_map, coords) (tile_id, sigma) =
    let next_coords = coords_next coords in
    (* Invert rotation before saving in ~data *)
    Map.set coords_map ~key:coords ~data:(tile_id, D4.(cps A3 sigma)), next_coords
  in
  let _, (coords_map, _) = go_forward_trans_clos_fold border_maps o_tile ~init:(coords_map, coords) ~f in
  coords_map

let chart_from_top_left border_maps top_left_o_tile =
  let tile_id, sigma = top_left_o_tile in
  let coords_next (x, y) = (x + 1, y) in
  let o_tile = (tile_id, D4.(cps A2 sigma)) in
  let f (coords_map, coords) (tile_id, sigma) =
    let next_coords = coords_next coords in
    (* Invert rotation before passing to chart_row_from_left *)
    let o_tile = tile_id, D4.(cps A2 sigma) in
    let coords_map = chart_row_from_left border_maps coords_map coords o_tile in
    coords_map, next_coords
  in
  let _, coords_map = go_forward_trans_clos_fold border_maps o_tile ~init:(Map.empty (module Coords), (0, 0)) ~f in
  coords_map

let rec parse_lines = (function
  | "" :: rest -> parse_lines rest
  | info :: l1 :: l2 :: l3 :: l4 :: l5 :: l6 :: l7 :: l8 :: l9 :: l10 :: rest ->
    let data = Array.of_list [l1; l2; l3; l4; l5; l6; l7; l8; l9; l10] in
    Map.set (parse_lines rest) ~key:(parse_info info) ~data
  | [] -> Map.empty (module Int)
  | (_ as input) -> raise (Illegal_input input)
)

let () =
  let lines = Stdio.In_channel.input_lines Stdio.stdin in
  let tiles = parse_lines lines in
  let border_maps = Map.fold ~init:(Map.empty (module Oriented_tile), Map.empty (module Int)) ~f:register_tile tiles in
  let o_tile, _ =
    let border_map, _ = border_maps in
    Map.max_elt_exn border_map
  in
  let o_tile = go_to_top_left border_maps o_tile in
  let coords_map, _ = chart_from_top_left border_maps o_tile in
  let f ~key ~data = printf "%s -> %s\n" (Sexp.to_string_hum (Coords.sexp_of_t key)) (Sexp.to_string_hum (Oriented_tile.sexp_of_t data)) in
  Map.iteri coords_map ~f