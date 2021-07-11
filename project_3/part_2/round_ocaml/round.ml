(* LIBRARY FUNCTIONS *)
let second lst = List.nth lst 1
let last lst = List.nth lst (List.length lst - 1)

module IntMap = Map.Make(struct type t = int let compare = compare end)

type inputVals = { 
  town_count:int; 
  car_count:int; 
  positions: int IntMap.t;
  populated_towns : int array;
  }

type townDistance = {
  town_id:int;
  max_distance:int;
  total_distance:int;
  furthest_idx:int;
}


(* ARGUMENT PARSING *)
let parse_firstline line = 
  let first_int vals = int_of_string (List.hd vals)
  and second_int vals = int_of_string (second vals)
  in
    let parse_args args = first_int args, second_int args 
    and splitted = String.split_on_char ' ' line
    in
      parse_args splitted 

let parse_secondline line = 
    let splitted = String.split_on_char ' ' line
in 
  List.map int_of_string splitted 

let get_frequency_table positions =
  let add_one x =
    match x with
    | Some v -> Some (v+1)
    | None -> Some 1 
  in
    let add_or_update freq_table x =
      IntMap.(update x add_one freq_table)
    in
      let rec partial_create lst freq_table =
        match lst with
        | [] -> freq_table
        | x::xs -> partial_create xs (add_or_update freq_table x)
      in
        partial_create positions IntMap.empty

let get_populated_towns freq_table town_count = 
  let result = Array.make(IntMap.cardinal freq_table) 0 in
    let rec partial_create idx current_town =
      if current_town == town_count
        then ()
        else 
          if IntMap.(mem current_town freq_table) 
          then begin
            result.(idx) <- current_town; 
            partial_create (idx+1) (current_town+1)
          end
          else partial_create idx (current_town+1)
    in
      partial_create 0 1; result

(* parses the file and returns an inputVals instance *)
let get_input_vals fn =
  let ic = open_in fn in
      let tc, cc = parse_firstline (input_line ic) 
      and positions = parse_secondline(input_line ic) in
        let freq_table = get_frequency_table positions in
          let populated_towns = get_populated_towns freq_table tc in
            { 
              town_count = tc; 
              car_count= cc; 
              positions = freq_table;
              populated_towns = populated_towns;
            }


(* ALGORITHMS *)
let get_distance townFrom townTo townCount = 
  let d = townTo - townFrom in
    if d >= 0 then d else townCount + d
  
let get_next_populated iv idx = (idx+ 1) mod (Array.length iv.populated_towns)

let is_valid town_distance = 
  town_distance.max_distance 
  - (town_distance.total_distance - town_distance.max_distance) <=1

let calculate_first_town iv= 
  let furthest_town= iv.populated_towns.(0) 
  and partial_sum k v acc = if k == 0 then acc else acc + (get_distance k 0 iv.town_count) * v
  in
    let maxDistance = get_distance iv.populated_towns.(furthest_town) 0 iv.town_count
    and totalDistance = IntMap.(fold partial_sum iv.positions 0)
    in
    {
      town_id = 0; 
      max_distance = maxDistance; 
      total_distance = totalDistance; 
      furthest_idx = 0 
    }

let calculate_town iv last =
  let current_town_id = last.town_id+1 in
  let furthest_idx, maxDistance = 
    if iv.populated_towns.(last.furthest_idx) == current_town_id 
    then let f_idx = get_next_populated iv last.furthest_idx in
        f_idx, get_distance iv.populated_towns.(f_idx) current_town_id iv.town_count
    else last.furthest_idx, last.max_distance+1
  in
    let current_town_car_count = if IntMap.(mem current_town_id iv.positions) 
      then IntMap.(find current_town_id iv.positions) 
      else 0
    in
      let totalDistance = last.total_distance + iv.car_count - iv.town_count * current_town_car_count
      in
        {
          town_id = current_town_id;
          max_distance = maxDistance;
          total_distance = totalDistance;
          furthest_idx = furthest_idx
        }

let solve iv = 
  let first_town = calculate_first_town iv in 
  let rec partial_shit lst idx =
    if idx == iv.town_count 
    then lst
    else let hd = calculate_town iv (List.hd lst) in partial_shit (hd::lst) (idx+1) 
  in
    let comparer acc curr = 
      if (is_valid curr) 
      && (not (is_valid acc) || curr.total_distance < acc.total_distance)
      then curr else acc
    in
      let get_solution distances = List.fold_left comparer (List.hd distances) distances 
      in
        get_solution (partial_shit [first_town] 1)

    
(* SOLUTION ENTRYPOINT *)
let () = 
  let solution = solve (get_input_vals Sys.argv.(1)) in
  Printf.printf "%d %d\n" solution.total_distance solution.town_id;
