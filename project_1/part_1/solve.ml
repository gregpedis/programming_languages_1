(* LIBRARY FUNCTIONS *)
let first lst = List.nth lst 0
let second lst = List.nth lst 1
let last lst = List.nth lst (List.length lst - 1)

let rec sum lst = 
  match lst with
    | [] -> 0
    | head::tail -> head + sum tail

let sum2 lst = List.fold_left (+) 0 lst

type inputVals = { hc:int; dc:int; days:int list }
type impIdx = { idx:int; vl:int }
let new_imp idx vl = { idx = idx; vl = vl }


(* ARGUMENT PARSING *)
let readfile fn =
  let ic = open_in fn in
    try
      let l1 = input_line ic
      and l2 = input_line ic
      in
        flush stdout;
        close_in ic;
        l1,l2
    with e ->
      close_in_noerr ic;
      raise e

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

let get_inputVals = 
  let filename = Sys.argv.(1)
  in
    let l1,l2 = readfile filename
    in 
      let dc, hc = get_params l1
      and days = get_days l2
      in
        { hc = hc; dc = dc; days = days }


(* ALGORITHMS *)        
let simplify_values values hc = 
  let simplify v = -1 * (v + hc)
  in 
    List.map simplify values

let get_prefixes l =
  let rec inner_prefixes l last_sum res =
    match l with
    | [] -> []
    | [a] ->  res @ [ a + last_sum ]
    | hd::tl -> 
      let new_sum = hd + last_sum 
      and new_res = res @ [ hd + last_sum ]
      in
        inner_prefixes tl new_sum new_res
        in
          0 :: inner_prefixes l 0 [] 
  
let find_leftmost_idx idxs target =
  let last_idx = List.length idxs - 1
  in
    let rec binary_search values target_value low high found =
      if low<high then 
        let mid = low + (high-low)/2 in
          if target_value >= (List.nth values mid).vl 
          then binary_search values target_value low (mid-1) mid
          else binary_search values target_value (mid+1) high found
      else (List.nth values found).idx
    in 
      binary_search idxs target 0 last_idx last_idx


let solve prefixes =
  let local_solve pfx pos idxs =
    if pfx >= (last idxs).vl
      then pos - find_leftmost_idx idxs pfx
      else 0
  in
    let rec inner_solve prefixes imp_idxs pos res =
      match prefixes with
        | [] -> res
        | [a] -> max res (local_solve a pos imp_idxs) 
        | hd::tl -> 
            let local_max = max res (local_solve hd pos imp_idxs)
            and new_imp = if hd < (last imp_idxs).vl then [new_imp pos hd] else [] 
            in
              inner_solve tl (imp_idxs @ new_imp) (pos+1) local_max
    in
      inner_solve prefixes [new_imp 0 0] 0 0


(* SOLUTION ENTRYPOINT *)
let () =
  let input_vals = get_inputVals 
  in
    let simplified = simplify_values input_vals.days input_vals.hc
    in
      let prefixes = get_prefixes simplified
      in
        print_endline (string_of_int (solve prefixes));
        (*
        print_int (solve prefixes)
        *)

