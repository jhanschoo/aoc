open Core

let t_desired = 2020

let compute_diff e ~t ~clock =
  (match Map.find clock e with
  | Some n -> (t - n)
  | None -> 0
  )

let rec process_initial initial ~t ~clock =
  (match initial with
  | [] -> Error ()
  | [i] -> Ok (t + 1, Map.set clock ~key:i ~data:t, compute_diff i ~t ~clock)
  | i :: rest -> process_initial rest ~t:(t + 1) ~clock:(Map.set clock ~key:i ~data:t)
  )

let rec compute ~t ~clock ~next =
  match Int.(t = t_desired) with
  | true -> next
  | false ->
    let next_next =
      match Map.find clock next with
      | Some n -> (t - n)
      | None -> 0
    in
    compute ~t:(t + 1) ~clock:(Map.set clock ~key:next ~data:t) ~next:next_next

let () =
  let res = Stdio.In_channel.input_lines Stdio.stdin in
  let _ = (
    let open Result.Let_syntax in
    let%bind l =
    match res with
    | [l] -> Ok l
    | _ -> Error ()
    in
    let initial = List.map (String.split l ~on:',') ~f:Int.of_string in
    let%bind (t, clock, next) = process_initial initial ~t:1 ~clock:(Map.empty (module Int)) in
    let next = compute ~t ~clock ~next in
    Ok (printf "%d\n" next)
  )
  in
  ()