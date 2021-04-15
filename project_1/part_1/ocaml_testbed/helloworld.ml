(* LIBRARY FUNCTIONS *)
let last lst = List.nth lst (List.length lst - 1)
type inputVals = { hc:int; dc:int; days:int list }


(* ALGORITHMS *)        
let get_simplified_prefixes l hc =
  let simplify v = -1 * (v + hc) in
    let partial_apply acc v =
      match acc with 
      | [] -> [simplify v]
      | [a] 
      | a :: _ -> (simplify v + a) :: acc
    in
      0 :: List.rev (List.fold_left partial_apply [] l)
      
let solve prefixes =
  if last prefixes >= 0 then (List.length prefixes - 1)
  else 
    let apply_left f acc v = 
      match acc with
      | [] -> [v]
      | [a] 
      | a :: _ -> (f a v) :: acc 
    in 
      let apply_right f v acc = apply_left f acc v in
      let valid_left = List.rev (List.fold_left (apply_left min) [] prefixes )
      and valid_right = List.fold_right (apply_right max) prefixes []
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
  let first_int vals = int_of_string (List.hd vals)
  and second_int vals = int_of_string (List.nth vals 1)
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
  let filename = Sys.argv.(1) in
  let l1,l2 = readfile filename in
    let dc, hc = get_params l1
    and days = get_days l2 in
      { hc = hc; dc=dc; days=days }


(* SOLUTION ENTRYPOINT *)
let () =
  let input_vals = get_inputVals in
      let prefixes = get_simplified_prefixes input_vals.days input_vals.hc in
      Printf.printf "%d\n" (solve prefixes);

