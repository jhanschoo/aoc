open Core

let process_seat seat_bsp =
  let f i prev c =
    match c with
    | 'B' | 'R' -> prev + Int.(2 ** (9 - i))
    | _ -> prev
  in
  String.foldi ~init:0 ~f seat_bsp

let () = Stdio.In_channel.input_lines Stdio.stdin
  |> List.map ~f:process_seat
  |> List.fold ~init:0 ~f:Int.max
  |> printf "%d\n"