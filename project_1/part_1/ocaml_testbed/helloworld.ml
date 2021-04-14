(* TESTBED PARAMETERS *)
let line1 = "11 3"
let line2 = "42 -10 8 1 11 -6 -12 16 -15 -11 13"
let length = 11
let hospital_count = 3


(* LIBRARY FUNCTIONS *)
let first lst = List.nth lst 0
let second lst = List.nth lst 1
let last lst = List.nth lst (List.length lst - 1)

type inputVals = { hc:int; dc:int; days:int list }


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
let get_simplified_prefixes l hc =
  let simplify v = -1 * (v + hc) in
    let rec inner_prefixes l last_sum res =
      match l with
      | [] -> []
      | [a] ->  res @ [ simplify a + last_sum ]
      | hd::tl -> 
        let new_sum = simplify hd + last_sum in
          let new_res = res @ [ new_sum ]
          in
            inner_prefixes tl new_sum new_res
            in
              0 :: inner_prefixes l 0 [] 


let some f acc v =
  match acc with
  | [] -> [v]
  | [a] -> acc @ [f a v]
  | _:: tl -> acc @ [f (last tl) v]


let solve prefixes =
  if last prefixes >= 0 then (List.length prefixes - 1)
  else 
    let partial_apply f acc v = 
      match acc with
      | [] -> [v]
      | [a] -> acc @ [f a v]
      | _:: tl -> acc @ [f (last tl) v]
    in
      let valid_left = List.fold_left (partial_apply min) [] prefixes 
      and valid_right = List.rev (List.fold_left (partial_apply max) [] (List.rev prefixes))
      in
        let rec rec_solve valid_left valid_right i j res =
          match (valid_left, valid_right) with
          | ([], _) -> res
          | (_, []) -> res
          | ([a], hd::tl) -> begin 
              if a < hd 
              then rec_solve valid_left tl i (j+1) (max res (j-i)) 
              else res end
          | (hd::tl, [a]) -> begin
              if hd < a 
              then max res (j-i)
              else rec_solve tl valid_right (i+1) j res end
          | (hd1::tl1, hd2::tl2) -> begin
              if hd1<hd2
              then rec_solve valid_left tl2 i (j+1) (max res (j-i))
              else rec_solve tl1 valid_right (i+1) j res end
        in
          (rec_solve valid_left valid_right 0 0 (-1)) 



(* SOLUTION ENTRYPOINT *)
let () =
  let input_vals = get_inputVals in
      let prefixes = get_simplified_prefixes input_vals.days input_vals.hc in
        Printf.printf "%d\n" (solve prefixes);

