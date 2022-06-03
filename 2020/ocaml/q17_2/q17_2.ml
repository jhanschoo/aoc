open Core;;
(* exception Exit;; *)

module T4 = struct
  type ('a, 'b, 'c, 'd) t = 'a * 'b * 'c * 'd [@@deriving sexp, typerep]

  let create a b c d = a, b, c, d

  (* lexicographic comparison  *)
  let compare ~cmp1 ~cmp2 ~cmp3 ~cmp4 (x, y, z, w) (x', y', z', w') =
    let c1 = cmp1 x x' in
    if c1 <> 0
    then c1
    else (
      let c2 = cmp2 y y' in
      if c2 <> 0 then c2 else (
        let c3 = cmp3 z z' in
        if c3 <> 0 then c3 else cmp4 w w'))
  ;;
end

module Int_4_tuple = struct
  module T = struct
    type t = Int.t * Int.t * Int.t * Int.t
    let compare = T4.compare ~cmp1:Int.compare ~cmp2:Int.compare ~cmp3:Int.compare ~cmp4:Int.compare
    let sexp_of_t = T4.sexp_of_t Int.sexp_of_t Int.sexp_of_t Int.sexp_of_t Int.sexp_of_t
  end
  include T
  include Comparator.Make (T)
end;;

let deltas =
  let p_delta = [-1;0;1] in
  List.fold p_delta ~init:[] ~f:(fun a x ->
    List.fold p_delta ~init:a ~f:(fun a y ->
      List.fold p_delta ~init:a ~f:(fun a z ->
        List.fold p_delta ~init:a ~f:(fun a w -> (x,y,z,w)::a)
        )
      )
    )
  |> List.filter ~f:(fun (x,y,z,w) -> x <> 0 || y <> 0 || z <> 0 || w <> 0)

let step points =
  let f ~key:(x,y,z,w) ~data:_ m =
    let f = function
      | Some c -> c + 1
      | None -> 1
    in
    let update_by_nb m p = Map.update m p ~f in
    List.fold deltas ~init:m ~f:(fun m (dx,dy,dz,dw) -> update_by_nb m (x+dx,y+dy,z+dz,w+dw))
  in
  let neighbour_counts = Map.fold points ~init:(Map.empty (module Int_4_tuple)) ~f
  in
  let f ~key ~data m =
    match data with
    (* inactive, c > 0 *)
    | `Left c ->
      if c = 3
        then Map.set m ~key ~data:()
      else m
    (* active, c = 0 *)
    | `Right () -> m
    | `Both (c,()) ->
      if c = 2 || c = 3
        then Map.set m ~key ~data:()
        else m
  in
  Map.fold2 neighbour_counts points ~init:(Map.empty (module Int_4_tuple)) ~f

let process_init_map line = List.foldi line ~init:[] ~f:(fun i l r -> String.foldi r ~init:l ~f:(fun j l c -> if Char.(c = '#') then (T4.create i j 0 0, ()) :: l else l))

let () =
  let open Result.Monad_infix in
  Stdio.In_channel.input_lines Stdio.stdin
    |> process_init_map
    |> Map.of_alist_or_error (module Int_4_tuple)
    >>| Base.Fn.apply_n_times ~n:6 step
    >>| Map.length
    >>| printf "%d\n"
    (* >>| Map.iter_keys ~f:(fun (i,j,k) -> printf "%d %d %d\n" i j k) *)
    |> (fun _ -> ())