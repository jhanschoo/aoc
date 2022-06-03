open Core;;
(* exception Exit;; *)

module Int_triple = struct
  module T = struct
    type t = Int.t * Int.t * Int.t
    let compare = Tuple3.compare ~cmp1:Int.compare ~cmp2:Int.compare ~cmp3:Int.compare
    let sexp_of_t = Tuple3.sexp_of_t Int.sexp_of_t Int.sexp_of_t Int.sexp_of_t
  end
  include T
  include Comparator.Make (T)
end;;

let deltas =
  let p_delta = [-1;0;1] in
  List.fold p_delta ~init:[] ~f:(fun a x ->
    List.fold p_delta ~init:a ~f:(fun a y ->
      List.fold p_delta ~init:a ~f:(fun a z -> (x,y,z)::a)
      )
    )
  |> List.filter ~f:(fun (x,y,z) -> x <> 0 || y <> 0 || z <> 0)

let step points =
  let f ~key:(x,y,z) ~data:_ m =
    let f = function
      | Some c -> c + 1
      | None -> 1
    in
    let update_by_nb m p = Map.update m p ~f in
    List.fold deltas ~init:m ~f:(fun m (dx,dy,dz) -> update_by_nb m (x+dx,y+dy,z+dz))
  in
  let neighbour_counts = Map.fold points ~init:(Map.empty (module Int_triple)) ~f
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
  Map.fold2 neighbour_counts points ~init:(Map.empty (module Int_triple)) ~f

let process_init_map line = List.foldi line ~init:[] ~f:(fun i l r -> String.foldi r ~init:l ~f:(fun j l c -> if Char.(c = '#') then (Tuple.T3.create i j 0, ()) :: l else l))

let () =
  let open Result.Monad_infix in
  Stdio.In_channel.input_lines Stdio.stdin
    |> process_init_map
    |> Map.of_alist_or_error (module Int_triple)
    >>| Base.Fn.apply_n_times ~n:6 step
    >>| Map.length
    >>| printf "%d\n"
    (* >>| Map.iter_keys ~f:(fun (i,j,k) -> printf "%d %d %d\n" i j k) *)
    |> (fun _ -> ())