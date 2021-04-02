(*let () = print_endline "Hello, World!" *)

let line1 = "11 3"
let line2 = "42 -10 8 1 11 -6 -12 16 -15 -11 13"

let length = 11
let hospital_count = 3

let get_params line = 
  let parse_args args =
    (int_of_string (List.nth args 0), int_of_string (List.nth args 1))
  and values = String.split_on_char ' ' line
  in
   parse_args values

let get_days line = 
  let values = String.split_on_char ' ' line
  in
    List.map int_of_string values

let simplifyValue v hc = -1 * (v + hc)

let simplifyValues vs hc = List.map (function n -> simplifyValue n hc) vs

let rec sum lst = 
  match lst with
    | [] -> 0
    | head::tail -> head + sum tail

let sum2 l = List.fold_left (+) 0 l

let prefixes l = 
  let rec rev_pref l =
    match l with 
     | [] -> []
     | [a] -> [a]
     | _::tl -> sum2 l :: rev_pref tl
  in 
  0 :: List.rev (rev_pref (List.rev l) )

type importantIndex = { idx:int; vl:int }
  
let addIfImportant e values =
  match values with
  | [] -> [e]
  | [a] -> if e.vl<a.vl then [a;e] else [a]
  | hd::tl -> 
      let last = List.nth tl (List.length tl - 1)
        in
         if e.vl<last.vl then hd::tl@[e] else hd::tl


