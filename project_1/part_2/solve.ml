(* LIBRARY FUNCTIONS *)
let second lst = List.nth lst 1
let last lst = List.nth lst (List.length lst - 1)

let sum1 lst = List.fold_left (+) 0 lst

type room = { x:int; y:int; dir: char }
type inpuVals = { sx:int; sy:int; rooms:room list list }

(* explodes a range from 0 to n-1 by applying f n times *)
let explode size f =
  if size <= 0 then []
  else let rec explode_inner f x max_x =
    let new_value = f x in
      if x==max_x then  [new_value]
      else new_value :: explode_inner f (x + 1) max_x
      in
        explode_inner f 0 (size-1)
   

(* ARGUMENT PARSING *)
let get_dimensions line = 
  let first_int vals = int_of_string (List.hd vals)
  and second_int vals = int_of_string (second vals)
  in
    let parse_args args = first_int args, second_int args 
    and splitted = String.split_on_char ' ' line
    in
      parse_args splitted 

let get_rooms_matrix ic sx sy = 
  let get_rooms_row ic x = 
    let row = input_line ic in
      let get_room x y = { x=x; y=y; dir=String.get row y } 
      in
      (* Explodes a row in rooms *)
        explode sy (get_room x) 
  in
      (* Explodes the array in room-rows *)
    explode sx (get_rooms_row ic)

let get_inputVals fn =
  let ic = open_in fn in
      let sx, sy = get_dimensions (input_line ic) in
        let rooms = get_rooms_matrix ic sx sy in
          { sx = sx; sy = sy; rooms = rooms }

(* ALGORITHMS *)
(* Maps an f to a list of rooms and returns Some to valid direction matches and None to invalid direction matches *)
let find_matches rooms d = 
  let get_valid room = if room.dir == d then Some room else None in
    List.map get_valid rooms

(* Gets the rooms on the first/last row and first/last column that exit the maze *)
let get_valid_perimeter_rooms rooms =
  let row_first = List.hd rooms 
  and column_first = List.map List.hd rooms 
  and row_last = last rooms
  and column_last = List.map last rooms  
  in
    find_matches row_first    'U' 
    @
    find_matches row_last     'D'
    @
    find_matches column_first 'L' 
    @
    find_matches column_last  'R' 
  
let get_room rooms x y dir sx sy = 
  let inside_x = x >= 0 && x < sx 
  and inside_y = y >= 0 && y < sy
  in
    if inside_x && inside_y then
      let room = (List.nth (List.nth rooms x) y) in
        if room.dir == dir then Some room else None
    else None

(*  Gets the amount of rooms for which the path ends to room *)
  let rec get_valid_count_by_room iv curr_room =
    match curr_room with
    | Some room -> begin
        let room_up    = get_room iv.rooms (room.x-1) room.y 'D' iv.sx iv.sy (* Does room above go down? *)
        and room_down  = get_room iv.rooms (room.x+1) room.y 'U' iv.sx iv.sy (* Does room below go up? *)
        and room_left  = get_room iv.rooms room.x (room.y-1) 'R' iv.sx iv.sy (* Does room to the left go right? *)
        and room_right = get_room iv.rooms room.x (room.y+1) 'L' iv.sx iv.sy (* Does room to the right go left? *)
        and f r = get_valid_count_by_room iv r
        in 1 + f room_up + f room_down + f room_left + f room_right
      end
    | None -> 0 

(*
Finds the valid perimeter rooms and then finds the rooms for which their path end on a valid perimeter room.
The solution is [AllRooms - ValidRooms]
*)
let solve iv = 
  let total_room_count = iv.sx * iv.sy
  and perimeter_rooms = get_valid_perimeter_rooms iv.rooms
  in
    let counts_by_room = List.map (get_valid_count_by_room iv) perimeter_rooms
    in
      total_room_count - sum1 counts_by_room

    
(* SOLUTION ENTRYPOINT *)
let () = 
  let solution = solve (get_inputVals Sys.argv.(1)) in
  Printf.printf "%d\n" solution;

