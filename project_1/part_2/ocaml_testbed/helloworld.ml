(* TESTBED PARAMETERS *)
let testfile = "../testcases/fairmaze.in1"
let dimensions_input = "2 2"
let rooms_input = "UL\nRL\n"


(* LIBRARY FUNCTIONS *)
let second lst = List.nth lst 1
let last lst = List.nth lst (List.length lst - 1)

type direction = Up | Down | Left | Right
exception Invalid_Direction

type room = { x:int; y:int; dir: direction }
type inpuVals = { size_x:int; size_y:int; rooms:room list list }

let get_direction c =
  match c with
  | 'U' -> Up
  | 'D' -> Down
  | 'L' -> Left
  | 'R' -> Right
  | _ -> raise Invalid_Direction

(* explodes a string to a list of 'a by applying f to each char *)
let explode_string s f = 
  let idx_to_a idx = f (String.get s idx) in
    List.init (String.length s) idx_to_a 
    

(* ARGUMENT PARSING *)
let get_dimensions line = 
  let first_int vals = int_of_string (List.hd vals)
  and second_int vals = int_of_string (second vals)
  in
    let parse_args args = first_int args, second_int args 
    and splitted = String.split_on_char ' ' line
    in
      parse_args splitted 

let get_rooms_matrix ic sx = 
  let get_rooms_row ic x = 
    let row = input_line ic in
      let get_room x y = { x=x; y=y; dir=get_direction (String.get row y) } 
      and sy = String.length row in
        List.init sy (get_room x)
  in
    List.init sx (get_rooms_row ic)

let get_inputVals fn =
  let ic = open_in fn in
    try
      let sx, sy = get_dimensions (input_line ic) in
        let rooms = get_rooms_matrix ic sx in
          close_in ic;
          { size_x = sx; size_y = sy; rooms = rooms }
    with e ->
      close_in_noerr ic;
      raise e


(* ALGORITHMS *)
let find_matches rooms d = 
  let is_valid room = room.dir == d in
    List.map (fun x -> Some x) (List.filter is_valid rooms)

let get_valid_perimeter_rooms rooms =
  let row_first = List.hd rooms
  and row_last = last rooms
  and column_first = List.map List.hd rooms 
  and column_last = List.map last rooms
  in
    find_matches row_first Up 
    @ find_matches row_last Down 
    @ find_matches column_first Left 
    @ find_matches column_last Right
  
let get_room rooms x y dir = 
  let inside_x = x >= 0 && x < List.length rooms
  and inside_y = y >= 0 && y< List.length (List.hd rooms)
  in
    if inside_x && inside_y then
      let room = (List.nth (List.nth rooms x) y) in
        if room.dir == dir then Some room else None
    else None

let rec get_valid_count_by_room rooms curr_room =
  match curr_room with
  | Some room -> begin
      let room_up    = get_room rooms (room.x-1) room.y Down
      and room_down  = get_room rooms (room.x+1) room.y Up
      and room_left  = get_room rooms room.x (room.y-1) Right
      and room_right = get_room rooms room.x (room.y+1) Left
      and f r = get_valid_count_by_room rooms r
      in 1 + f room_up + f room_down + f room_left + f room_right
    end
  | None -> 0

let solve iv = 
  let total_room_count = iv.size_x * iv.size_y
  and perimeter_rooms = get_valid_perimeter_rooms iv.rooms
  and sum lst = List.fold_left (+) 0 lst
  in
    let counts_by_room = List.map (get_valid_count_by_room iv.rooms) perimeter_rooms
    in
      total_room_count - sum counts_by_room

    
(* SOLUTION ENTRYPOINT *)
let () = 
  let solution = solve (get_inputVals Sys.argv.(1)) in
    print_endline ( string_of_int solution )

