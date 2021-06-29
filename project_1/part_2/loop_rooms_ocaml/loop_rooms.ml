(* LIBRARY FUNCTIONS *)
let second lst = List.nth lst 1
let last lst = List.nth lst (List.length lst - 1)

type input_vals = { sx:int; sy:int; rooms:char array array }

(* ARGUMENT PARSING *)
let get_dimensions line = 
  let first_int vals = int_of_string (List.hd vals)
  and second_int vals = int_of_string (second vals)
  in
    let parse_args args = first_int args, second_int args 
    and splitted = String.split_on_char ' ' line
    in
      parse_args splitted 

let get_rooms ic sx sy = 
  (* the resulting array of rooms *)
  let result = Array.make_matrix sx sy '-'  
  (* this applies a function to a range of (1 to n-1) *)
  and loop f max_x = 
  let rec rec_loop f x = 
  (* short circuit/recursion end condition *)
  if x < 0 || x >= max_x then ()
  (* this is the loop *)
  else begin f x; rec_loop f (x+1); () end in
    rec_loop f 0 in
    (* sets a row of rooms *)
    let set_rooms_row ic x = 
      let row = input_line ic in
      (* sets a room *)
      let set_room x y = result.(x).(y) <- String.get row y; () in
      (* loops set_room for a row *)
      loop (set_room x) sy; ()
    in
    (* loops set_room_row for all the lines *)
      loop (set_rooms_row ic) sx;
      result

(* parses the file and returns an inputVals instance *)
let get_input_vals fn =
  let ic = open_in fn in
      let sx, sy = get_dimensions (input_line ic) in
        { sx = sx; sy = sy; rooms = get_rooms ic sx sy }

(* ALGORITHMS *)

(* find matches in a row of rooms  *)
(* and returns Some (x,y) or None based on direction specified *)
let find_matches_h rooms dir i = 
  let get_valid j room = if room == dir then Some (i,j) else None in
    Array.mapi get_valid rooms


(* find matches in a column of rooms  *)
(* and returns Some (x,y) or None based on direction specified *)
let find_matches_v rooms dir j = 
  let get_valid i room = if room == dir then Some (i,j) else None in
    Array.mapi get_valid rooms

(* Gets the rooms on the first/last row and first/last column that exit the maze *)
let get_valid_perimeter_rooms rooms sx sy =
  let first_row = rooms.(0)
  and last_row = rooms.(sx-1) 
  and first_column = Array.map (fun row -> row.(0)) rooms
  and last_column = Array.map (fun row -> row.(sy-1)) rooms
  in
    [|
      (find_matches_h first_row 'U' 0)        ;
      (find_matches_h last_row 'D' (sx-1))    ;
      (find_matches_v first_column 'L' 0)     ;
      (find_matches_v last_column 'R' (sy-1))
      |]

(* gets a room if it is inside bounds and matches the specified direction, else None*)
let get_room iv x y dir = 
  let inside_x = x >= 0 && x < iv.sx 
  and inside_y = y >= 0 && y < iv.sy
  in
    if inside_x && inside_y then
      let room = iv.rooms.(x).(y) in
        if room == dir then Some (x,y) else None
    else None

(*  Gets the amount of rooms for which the path ends to room *)
  let rec get_valid_count_by_room iv curr_room =
    (* Here we use pattern-matching on an [Option Room] *)
    (* to add either 1 and recurse or 0 and exit early *) 
    match curr_room with
    | Some room -> begin
        let x,y = room in
        let room_up    = get_room iv (x-1) y 'D' (* Does room above go down? *)
        and room_down  = get_room iv (x+1) y 'U' (* Does room below go up? *)
        and room_left  = get_room iv x (y-1) 'R' (* Does room to the left go right? *)
        and room_right = get_room iv x (y+1) 'L' (* Does room to the right go left? *)
        and f r = get_valid_count_by_room iv r
        in 1 + f room_up + f room_down + f room_left + f room_right
      end
    | None -> 0 

(*
Finds the valid perimeter rooms and then finds the rooms 
for which their path end on a valid perimeter room.
The solution is [AllRooms - ValidRooms]
*)
let solve iv = 
  let total_room_count = iv.sx * iv.sy
  and perimeter_rooms = get_valid_perimeter_rooms iv.rooms iv.sx iv.sy
  and sum1 a = Array.fold_left (+) 0 a
  in
    (* this monstrosity applies the get_valid_count_by_room on some rooms  *)
    (* and returns the sum of the valid rooms *)
    (* First it maps every (x,y) to the amount of valid rooms as an int *)
    (* Secondly it sums the int array into a single int *)
    (* (x,y) option array -> int array -> int *)
    let partial_apply some_rooms = sum1 (Array.map (get_valid_count_by_room iv) some_rooms) in
    let counts_by_room = Array.map partial_apply perimeter_rooms
    in
      total_room_count - sum1 counts_by_room

    
(* SOLUTION ENTRYPOINT *)
let () = 
  let solution = solve (get_input_vals Sys.argv.(1)) in
  Printf.printf "%d\n" solution;

