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
  Array.fold ~init:0 ~f

let register_border_hash (border_map, rev_border_map) hash tile_id sigma =
  let border_map = Map.set border_map ~key:(tile_id, sigma) ~data:hash in
  let rev_border_map = Map.update rev_border_map hash ~f:(function
    | Some l -> (tile_id, sigma)::l
    | None -> [(tile_id, sigma)]
  ) in
  (border_map, rev_border_map)

let register_border border_maps border tile_id sigma =
  register_border_hash border_maps (hash_border border) tile_id sigma

  (* Convention: A rotates a square tau/4 anti-clockwise, B flips a square horizontally across the midpoint vertical axis *)
let register_tile ~key ~(data: char array array) border_maps =
  (* topleft to topright *)
  let border_maps = register_border border_maps data.(0) key D4.E in
  (* topright to topleft *)
  let border_maps = register_border border_maps (Array.rev data.(0)) key D4.B in
  let right = Array.create ~len:10 ' ' in
  let () = Array.iteri data ~f:(fun i r -> right.(i) <- r.(9)) in
  (* topright to bottomright *)
  let border_maps = register_border border_maps right key D4.A in
  (* bottomright to topright *)
  let border_maps = register_border border_maps (Array.rev right) key D4.BA in
  (* bottomright to bottomleft *)
  let border_maps = register_border border_maps (Array.rev data.(9)) key D4.A2 in
  (* bottomleft to bottomright *)
  let border_maps = register_border border_maps data.(9) key D4.BA2 in
  let left = Array.create ~len:10 ' ' in
  let () = Array.iteri data ~f:(fun i r -> left.(9 - i) <- r.(0)) in
  (* bottomleft to topleft *)
  let border_maps = register_border border_maps left key D4.A3 in
  (* topleft to bottomleft *)
  let border_maps = register_border border_maps (Array.rev left) key D4.BA3 in
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
      if Oriented_tile.equal o_tile_1 o_tile then o_tile_2 else
        if Oriented_tile.equal o_tile_2 o_tile then o_tile_1 else
          raise Illegal_argument
      in
      Some (next_tile_id, D4.(cps BA2 next_sigma))
    | [o_tile_o] -> if Oriented_tile.equal o_tile_o o_tile then None else raise Illegal_argument
    | _ -> raise Illegal_argument
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

let apply_permutation sigma tile =
  let open D4 in
  (* Define permutations in terms of B and A... inefficient, but necessary unless delving into metaprogramming *)
  let a tile =
    let len = Array.length tile in
    let next_tile = Array.make_matrix ~dimx:len ~dimy:len ' ' in
    for x = 0 to len - 1 do
      for y = 0 to len - 1 do
        next_tile.(x).(y) <- tile.(y).(len - 1 - x)
      done
    done;
    next_tile
  in
  let b tile =
    let len = Array.length tile in
    let next_tile = Array.make_matrix ~dimx:len ~dimy:len ' ' in
    for x = 0 to len - 1 do
      for y = 0 to len - 1 do
        next_tile.(x).(y) <- tile.(x).(len - 1 - y)
      done
    done;
    next_tile
  in
  (match sigma with
  | E -> tile
  | A -> a tile
  | A2 -> a (a tile)
  | A3 -> a (a (a tile))
  | B -> b tile
  | BA -> b (a tile)
  | BA2 -> b (a (a tile))
  | BA3 -> b (a (a (a tile)))
  )

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
    let data = Array.of_list (List.map ~f:String.to_array [l1; l2; l3; l4; l5; l6; l7; l8; l9; l10]) in
    Map.set (parse_lines rest) ~key:(parse_info info) ~data
  | [] -> Map.empty (module Int)
  | (_ as input) -> raise (Illegal_input input)
)

let map_size coords_map =
  let f ~key:(x, _) ~data:_ maximum = max x maximum in
  (Map.fold coords_map ~init:0 ~f) + 1

let assemble_map coords_map tile_map =
  let ij_len = map_size coords_map in
  let xy_len = Array.length (let _, tile = Map.max_elt_exn tile_map in tile) in
  let len = ij_len * xy_len in
  let map = Array.make_matrix ~dimx:len ~dimy:len ' ' in
  for i = 0 to ij_len - 1 do
    for j = 0 to ij_len - 1 do
      let tile_id, sigma = Map.find_exn coords_map (i, j) in
      let tile = Map.find_exn tile_map tile_id in
      let tile = apply_permutation sigma tile in
      for x = 0 to xy_len - 1 do
        for y = 0 to xy_len - 1 do
          map.(i * xy_len + x).(j * xy_len + y) <- tile.(x).(y)
        done
      done
    done
  done;
  map

(* let print_matrix mat =
  Array.iter mat ~f:(fun row -> printf "%s\n" (String.of_char_list (Array.to_list row))); printf "\n" *)

let monster_string =
"                  # 
#    ##    ##    ###
 #  #  #  #  #  #   "

let monster =
  let monster_lines = String.split_lines monster_string in
  let f x y coords_set c = (match c with '#' -> Set.add coords_set (x, y) | _ -> coords_set) in
  let f x coords_set line = String.foldi line ~init:coords_set ~f:(f x) in
  List.foldi monster_lines ~init:(Set.empty (module Coords)) ~f

let monster_size = Set.length monster

let mem_monster_at chart (x, y) =
  Set.for_all monster ~f:(fun (dx, dy) -> Char.(chart.(x + dx).(y + dy) = '#'))

let count_monsters chart =
  let x_end = Array.length chart - 3 in
  let y_end = Array.length (chart.(0)) - 20 in
  let xs = Sequence.init x_end ~f:Fn.id in
  let ys = Sequence.init y_end ~f:Fn.id in
  let coords = Sequence.cartesian_product xs ys in
  Sequence.fold coords ~init:0 ~f:(fun count coords -> if mem_monster_at chart coords then count + 1 else count)

let count_sharps chart =
  Array.sum (module Int) chart ~f:(Array.count ~f:(Char.(=) '#'))

let trim_tiles tile_map =
  let f tile =
    let len = Array.length tile in
    let ret = Array.make_matrix ~dimx:(len - 2) ~dimy:(len - 2) ' ' in
    for x = 0 to len - 3 do
      for y = 0 to len - 3 do
        ret.(x).(y) <- tile.(x + 1).(y + 1)
      done
    done;
    ret
  in
  Map.map tile_map ~f

let () =
  let lines = Stdio.In_channel.input_lines Stdio.stdin in
  let tile_map = parse_lines lines in
  let border_maps = Map.fold ~init:(Map.empty (module Oriented_tile), Map.empty (module Int)) ~f:register_tile tile_map in
  let o_tile, _ =
    let border_map, _ = border_maps in
    Map.max_elt_exn border_map
  in
  let o_tile = go_to_top_left border_maps o_tile in
  let coords_map, _ = chart_from_top_left border_maps o_tile in
  let tile_map = trim_tiles tile_map in
  let chart = assemble_map coords_map tile_map in
  let f sigma =
    let chart = apply_permutation sigma chart in
    (
      match count_monsters chart with
      | 0 -> ()
      | n -> printf "%d" ((count_sharps chart) - n * monster_size)
    )
  in
  List.iter D4.all ~f;
  printf "\n"
