open Core

let trans_m4_5 =
  [|
    [|1.; 0.; -4.5|];
    [|0.; 1.; -4.5|];
    [|0.; 0.; 1.|]
  |]

let trans_p4_5 =
  [|
    [|1.; 0.; 4.5|];
    [|0.; 1.; 4.5|];
    [|0.; 0.; 1.|]
  |]

let m_mul m1 m2 =
  let dimx = Array.length m1
  and dimy = Array.length (m2.(0)) in
  let r = Array.make_matrix ~dimx:(Array.length m1) ~dimy:(Array.length (m2.(0))) 0. in
  for x = 0 to dimx - 1 do
    for y = 0 to dimy - 1 do
      let f t = Float.(m1.(x).(t) * m2.(t).(y)) in
      let s = List.sum (module Float) (List.range 0 (Array.length m2)) ~f in
      r.(x).(y) <- s
    done
  done;
  r

let mv_mul m v =
  let r = Array.create ~len:(Array.length m.(0)) 0. in
  for i = 0 to Array.length v - 1 do
    let f t = Float.(m.(i).(t) * v.(t)) in
    let sum = List.sum (module Float) (List.range 0 (Array.length v)) ~f in
    r.(i) <- sum
  done;
  r

let [@warning "-32"]  apply_trf img trf =
  let r = Array.copy_matrix img in
  let dimx = Array.length img
  and dimy = Array.length (img.(0)) in
  for x = 0 to dimx - 1 do
    for y = 0 to dimy - 1 do
      let xy = mv_mul trf [|Float.of_int x; Float.of_int y; 1.|] in
      r.(Int.of_float(xy.(0))).(Int.of_float(xy.(1))) <- img.(x).(y)
    done
  done;
  printf "\n";
  r

let r1 =
  [|
    [|0.; -1.; 0.|];
    [|1.; 0.; 0.|];
    [|0.; 0.; 1.|]
  |]

let r2 = m_mul r1 r1
let r3 = m_mul r1 r2
let r0 = m_mul r1 r3

let s0 =
  [|
    [|1.; 0.; 0.|];
    [|0.; -1.; 0.|];
    [|0.; 0.; 1.|]
  |]
let s1 = m_mul r1 s0
let s2 = m_mul r2 s0
let s3 = m_mul r3 s0

let [@warning "-8"] [r0;r1;r2;r3;s0;s1;s2;s3] = List.map [r0;r1;r2;r3;s0;s1;s2;s3] ~f:(fun t -> m_mul trans_p4_5 (m_mul t trans_m4_5));;

let () =
  let _res = Stdio.In_channel.input_lines Stdio.stdin in
  let sample = [|
    [|0; 1; 2; 3; 4; 5; 6; 7; 8; 9|];
    [|10; 11; 12; 13; 14; 15; 16; 17; 18; 19|];
    [|20; 21; 22; 23; 24; 25; 26; 27; 28; 29|];
    [|30; 31; 32; 33; 34; 35; 36; 37; 38; 39|];
    [|40; 41; 42; 43; 44; 45; 46; 47; 48; 49|];
    [|50; 51; 52; 53; 54; 55; 56; 57; 58; 59|];
    [|60; 61; 62; 63; 64; 65; 66; 67; 68; 69|];
    [|70; 71; 72; 73; 74; 75; 76; 77; 78; 79|];
    [|80; 81; 82; 83; 84; 85; 86; 87; 88; 89|];
    [|90; 91; 92; 93; 94; 95; 96; 97; 98; 99|];
  |] in
  let f a = printf "%s\n" (Sexp.to_string_hum (Array.sexp_of_t Int.sexp_of_t a)) in
  let f m = Array.iter ~f (apply_trf sample m) in
  List.iter ~f [r0;r1;r2;r3;s0;s1;s2;s3]
