open Core
exception Exit;;

Set.of_list (module Int);;

module Int_tuple_comparator = struct
  module T = struct
    type t = Int.t * Int.t * Int.t
    let compare = Tuple3.compare ~cmp1:Int.compare ~cmp2:Int.compare ~cmp3:Int.compare
    let sexp_of_t = Tuple3.sexp_of_t Int.sexp_of_t Int.sexp_of_t Int.sexp_of_t
  end
  include T
  include Comparator.Make (T)
end;;

let process_map line = List.foldi line ~init:[] ~f:(fun i l r -> String.foldi r ~init:l ~f:(fun j l c -> if Char.(c = '#') then Tuple.T3.create i j 0 :: l else l))

let () = Stdio.In_channel.input_lines Stdio.stdin
  |> process_map
  |> Set.of_list (module Int_tuple_comparator)
  |> (fun _ -> ())