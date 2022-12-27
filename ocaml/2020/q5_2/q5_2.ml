open Core

let process_seat seat_bsp =
  let f i prev c =
    match c with
    | 'B' | 'R' -> prev + Int.(2 ** (9 - i))
    | _ -> prev
  in
  String.foldi ~init:0 ~f seat_bsp

let rec find_user_seat = function
| l :: (r :: _ as rest) ->
  if l + 1 = r then find_user_seat rest
  else l + 1
| _ -> -1

let () = Stdio.In_channel.input_lines Stdio.stdin
  |> List.map ~f:process_seat
  |> List.sort ~compare:Int.compare
  |> find_user_seat
  |> printf "%d\n"
