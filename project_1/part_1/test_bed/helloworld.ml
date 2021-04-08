(*let () = print_endline "Hello, World!" *)

(*
let line1 = "11 3"
let line2 = "42 -10 8 1 11 -6 -12 16 -15 -11 13"
*)

let line1 = "20 5"
let line2 = "-50 -68 -2 -26 -46 -12 22 57 -12 59 26 -30 -33 -63 -100 23 1 -76 -58 84"

let length = 11
let hospital_count = 3

let first lst = List.nth lst 0
let second lst = List.nth lst 1
let last lst = List.nth lst (List.length lst - 1)

type importantIndex = { idx:int; vl:int }

let new_imp idx vl = {idx=idx; vl = vl}

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

(*
let prefixes l = 
  let rec rev_pref l =
    match l with 
     | [] -> []
     | [a] -> [a]
     | _::tl -> sum2 l :: rev_pref tl
  in 
    0 :: List.rev (rev_pref (List.rev l) )
*)

let get_prefixes l =
  let rec inner_prefixes l last_hd last_idx res =
    match l with
    | [] -> []
    | [a] ->  res @ [ a+last_hd ]
    | hd::tl -> 
      res @ [ hd+last_hd ] @ inner_prefixes tl (hd+last_hd) (last_idx+1) res 
  in
    0 :: inner_prefixes l 0 0 [] 
  
let addIfImportant e values =
  match values with
    | [] -> [e]
    | [a] -> if e.vl<a.vl then [a;e] else [a]
    | hd::tl -> 
      let lst = last tl 
      in
         if e.vl<lst.vl then hd::tl@[e] else hd::tl

let find_leftmost_idx values target =
  let last_idx = List.length values - 1
  and last_imp_idx = (last values).idx
  in
    let rec binary_search values target_value low high found =
      if low<high then 
        let mid = low + (high-low)/2 in
          if target_value - (List.nth values mid).vl >=0
          then binary_search values target_value low (mid-1) (List.nth values mid).idx
          else binary_search values target_value (mid+1) high found
      else found
    in 
      binary_search values target 0  last_idx last_imp_idx


let solve prefixes =
  let local_solve pfx pos idxs =
    if pfx - (last idxs).vl < 0 
    then 0 
    else pos - find_leftmost_idx idxs pfx
  in
    let rec inner_solve prefixes imp_idxs res pos =
      match prefixes with
        | [] -> res
        | [a] -> max res (local_solve a pos imp_idxs) 
        | hd::tl -> 
            let local_max = max res (local_solve hd pos imp_idxs)
            and new_imp = if hd < (last imp_idxs).vl then [new_imp pos hd] else [] 
            in
              inner_solve tl (imp_idxs @ new_imp) local_max (pos+1) 
    in
      inner_solve prefixes [new_imp 0 0] 0 0

