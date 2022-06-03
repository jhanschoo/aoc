open Core;;
(* exception Exit;; *)

let preamble_length = 25

let () =
  let lines = Stdio.In_channel.input_lines Stdio.stdin in
  let numbers = List.map lines ~f:Int.of_string in
  let (preamble_raw, rest) = List.split_n numbers preamble_length in
  let preamble_list = Linked_queue.of_list preamble_raw in
  let preamble = Set.of_list (module Int) preamble_raw in
  let test_preamble_with_indices preamble l r total =
    let open Option.Let_syntax in
    let%bind nl = Set.nth preamble l in
    let%bind nr = Set.nth preamble r in
    Some (Int.compare (nl + nr) total)
  in
  let rec test_preamble preamble l r total = if l >= r then false else
    let c = test_preamble_with_indices preamble l r total in
    match c with
    | None -> false
    | Some c ->
      if c < 0 then test_preamble preamble (l+1) r total else
        if c > 0 then test_preamble preamble l (r-1) total else
          true
  in
  let f preamble v =
    match test_preamble preamble 0 (preamble_length-1) v with
    | false -> Error v
    | true ->
      let preamble = Set.remove preamble (Linked_queue.dequeue_exn preamble_list) in
      let preamble = Set.add preamble v in
      let () = Linked_queue.enqueue preamble_list v in
      Ok preamble
  in
  let res = List.fold_result rest ~init:preamble ~f in
  match res with
  | Ok _ -> ()
  | Error v -> printf "%d\n" v