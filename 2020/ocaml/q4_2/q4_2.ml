open Core
exception Empty_list

let lines_to_passport_field_pairs lines =
  let f passports line =
    match line with
    | "" -> [] :: passports
    | s -> match passports with
            | [] -> raise Empty_list
            | (passport :: rest) ->
              (s :: passport) :: rest
  and
  split_fields passport =
    passport
    |> List.map ~f:(String.split ~on:':')
  in
  lines
  |> List.fold ~init:[[]] ~f
  (* return of List.fold: list of lists of strings, each string is a line containing several fields of a passport entry, with the fields separated by space, and a passport spans one or more lines *)
  |> List.map ~f:(String.concat ~sep:" ")
  (* return of List.map: list of strings, each string is a line containing all fields of a passport entry, with the fields separated by space *)
  |> List.map ~f:(String.split ~on:' ')
  (* return of List.map: list of list of strings, each string is a field of a passport *)
  |> List.map ~f:(split_fields)

let validate_field past field =
match field with
| ["byr"; y] ->
  let y = Int.of_string y
  in
  if (y >= 1920 && y <= 2002)
    then Ok (past + 1)
    else Ok past
| ["iyr"; y] ->
  let y = Int.of_string y
  in
  if (y >= 2010 && y <= 2020)
    then Ok (past + 1)
    else Ok past
| ["eyr"; y] ->
  let y = Int.of_string y
  in
  if (y >= 2020 && y <= 2030)
    then Ok (past + 1)
    else Ok past
| ["hgt"; h] ->
  if (String.is_suffix h ~suffix:"cm")
    then
      let h = Int.of_string (String.drop_suffix h 2)
      in
      if (h >= 150 && h <= 193)
        then Ok (past + 1)
        else Ok past
    else
      if (String.is_suffix h ~suffix:"in")
        then
          let h = Int.of_string (String.drop_suffix h 2)
          in
          if (h >= 59 && h <= 76)
            then Ok (past + 1)
            else Ok past
        else Ok past
| ["hcl"; hcl] ->
  if (String.is_prefix hcl ~prefix:"#" && (String.for_all (String.drop_prefix hcl 1) ~f:(Char.is_hex_digit_lower)))
    then Ok (past + 1)
    else Ok past
| ["ecl"; ecl] ->
  if (List.exists ["amb"; "blu"; "brn"; "gry"; "grn"; "hzl"; "oth"] ~f:(fun e -> String.(ecl = e)))
    then Ok (past + 1)
    else Ok past
| ["pid"; pid] ->
  if (String.length pid = 9 && (String.for_all pid ~f:(Char.is_digit)))
    then Ok (past + 1)
    else Ok past
| ["cid"; _] -> Ok past
| _ -> Ok past


let validate_passport passport =
  let open Result
  in
  passport
  |> List.fold_result ~init:0 ~f:(validate_field)
  >>= (fun n -> if (n = 7) then Ok () else Error ())
  |> Result.is_ok

let () = Stdio.In_channel.input_lines Stdio.stdin
    |> lines_to_passport_field_pairs
    |> List.map ~f:(validate_passport)
    |> List.filter ~f:Base.Fn.id
    |> List.length
    |> printf "%d\n"
