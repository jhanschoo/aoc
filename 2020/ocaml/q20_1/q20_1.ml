open Core

let trans_m4 =
  [|
    [|1; 0; -4|];
    [|0; 1; -4|];
    [|0; 0; 1|]
  |]

let trans_p4 =
  [|
    [|1; 0; 4|];
    [|0; 1; 4|];
    [|0; 0; 1|]
  |]

let m_mul m1 m2 =
  let dimx = Array.length m1
  and dimy = Array.length (m2.(0)) in
  let r = Array.make_matrix ~dimx:(Array.length m1) ~dimy:(Array.length (m2.(0))) 0 in
  for x = 0 to dimx - 1 do
    for y = 0 to dimy - 1 do
      let f t = m1.(x).(t) * m2.(t).(y) in
      let s = List.sum (module Int) (List.range 0 (Array.length m2)) ~f in
      r.(x).(y) <- s
    done
  done;
  r

let mv_mul m v =
  let r = Array.create ~len:(Array.length m) 0 in
  for i = 0 to Array.length v - 1 do
    let f t = m.(t).(i) * v.(t) in
    let sum = List.sum (module Int) (List.range 0 (Array.length v)) ~f in
    r.(i) <- sum
  done;
  r

let apply_trf img trf =
  let r = Array.copy_matrix img in
  let dimx = Array.length img
  and dimy = Array.length (img.(0)) in
  for x = 0 to dimx - 1 do
    for y = 0 to dimy - 1 do
      let xy = mv_mul trf [|x;y;1|] in
      r.(xy.(0)).(xy.(1)) <- img.(x).(y)
    done
  done;
  r

let r1 =
  [|
    [|0; -1; 0|];
    [|1; 0; 0|];
    [|0; 0; 1|]
  |]

let r2 = m_mul r1 r1
let r3 = m_mul r1 r2
let r0 = m_mul r1 r3

let s0 =
  [|
    [|1; 0; 0|];
    [|0; -1; 0|];
    [|0; 0; 1|]
  |]
let s1 = m_mul r1 s0
let s2 = m_mul r2 s0
let s3 = m_mul r3 s0

let [@warning "-8"] [r0;r1;r2;r3;s0;s1;s2;s3] = List.map [r0;r1;r2;r3;s0;s1;s2;s3] ~f:(fun t -> m_mul trans_p4 (m_mul t trans_m4));;

let () =
  let _res = Stdio.In_channel.input_lines Stdio.stdin in
  List.iter [r0;r1;r2;r3;s0;s1;s2;s3] ~f:(fun m -> printf "%s\n\n" (Sexp.to_string_hum (Array.sexp_of_t (Array.sexp_of_t Int.sexp_of_t) m)))
  