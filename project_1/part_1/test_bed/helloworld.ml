(*let () = print_endline "Hello, World!" *)

let line1 = "11 3"
let line2 = "42 -10 8 1 11 -6 -12 16 -15 -11 13"

let length = 11
let hospital_count = 3

let first lst = List.nth lst 0
let second lst = List.nth lst 1
let last lst = List.nth lst (List.length lst - 1)

type importantIndex = { idx:int; vl:int }

let get_params line = 
  let first_int vals = int_of_string (first vals)
  and second_int vals = int_of_string (second vals)
  in
    let parse_args args = first_int args, second_int args 
    and values = String.split_on_char ' ' line
    in
      parse_args values

let get_days line = 
  let values = String.split_on_char ' ' line
  in
    List.map int_of_string values


let simplifyValues vals hc = 
  let sv v = -1 * (v + hc)
  in 
    List.map sv vals 

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

  
let addIfImportant e values =
  match values with
    | [] -> [e]
    | [a] -> if e.vl<a.vl then [a;e] else [a]
    | hd::tl -> 
      let lst = last tl 
      in
         if e.vl<lst.vl then hd::tl@[e] else hd::tl

let binary_search idxs p = 0         

let local_solve p idxs pos =
  if p - last idxs < 0 then 0 
  else pos - binary_search idxs p

let rec solve prefs imp_idxs res pos =
  match prefs with
    | [] -> res
    | [a] -> max res (local_solve a imp_idxs pos) 
    | hd::tl -> 
        let local_max = max res (local_solve hd imp_idxs pos)
        and new_imp = if hd < last imp_idxs then [{idx=pos;vl=hd}] else [] 
        in
          solve tl (imp_idxs @ new_imp) local_max (pos+1) 


