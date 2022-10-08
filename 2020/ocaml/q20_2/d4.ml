open Base

module T = struct
  type t =
    | E | A | A2 | A3
    | B | BA | BA2 | BA3
    [@@deriving compare, equal, sexp_of]
end
include T
include Comparator.Make(T)

let inv_of = function
  | E -> E
  | A -> A3
  | A2 -> A2
  | A3 -> A
  | B -> B
  | BA -> BA
  | BA2 -> BA2
  | BA3 -> BA3

let cps t u =
  (match t, u with
    | E, _ -> u
    | _, E -> t
    | A, A3 | A2, A2 | A3, A | B, B | BA, BA | BA2, BA2 | BA3, BA3 -> E
    | A2, A3 | A3, A2 | B, BA | BA, BA2 | BA2, BA3 | BA3, B -> A
    | A, A | A3, A3 | B, BA2 | BA, BA3 | BA2, B | BA3, BA -> A2
    | A, A2 | A2, A | B, BA3 | BA, B | BA2, BA | BA3, BA2 -> A3
    | A, BA | A2, BA2 | A3, BA3 | BA, A3 | BA2, A2 | BA3, A -> B
    | A, BA2 | A2, BA3 | A3, B | B, A | BA2, A3 | BA3, A2 -> BA
    | A, BA3 | A2, B | A3, BA | B, A2 | BA, A | BA3, A3 -> BA2
    | A, B | A2, BA | A3, BA2 | B, A3 | BA, A2 | BA2, A -> BA3
  )

let a_of = (cps A)
let b_of = (cps B)
let right_of = (cps A)

let trans_opp_of = (cps BA2)