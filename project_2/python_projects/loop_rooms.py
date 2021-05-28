import sys
from collections import deque as Stack

# data container for input variables


class InputVals:
    def __init__(self, sx, sy, rooms):
        self.sx = sx
        self.sy = sy
        self.rooms = rooms

    def get_total(self):
        return self.sx * self.sy

# reads a file and returns an instance of input vals.


def parse_file(fn):
    with open(fn) as f:
        lines = f.readlines()
        dimensions = [int(x) for x in lines[0].split(" ")]
        return InputVals(dimensions[0], dimensions[1], lines[1:])


# finds all the rooms on the first/last row/column that "look" outside.
def find_boundary_valid_rooms(input_vals):
    valid = []

    # first/last column
    for i in range(input_vals.sx):
        if input_vals.rooms[i][0] == "L":
            valid.append((i, 0))
        if input_vals.rooms[i][input_vals.sy-1] == "R":
            valid.append((i, input_vals.sy-1))

    # first/last row
    for j in range(input_vals.sy):
        if input_vals.rooms[0][j] == "U":
            valid.append((0, j))
        if input_vals.rooms[input_vals.sx-1][j] == "D":
            valid.append((input_vals.sx-1, j))
    return valid


# foreach room, calculate all the rooms that end-up on this room recursively.
def get_valid_room_count_by_room(room_x, room_y, iv):
    count = 0
    remaining = Stack()
    remaining.append((room_x, room_y))

    while remaining:
        x, y = remaining.pop()
        count += 1

        if x-1 >= 0 and iv.rooms[x-1][y] == "D":
            remaining.append((x-1, y))

        if x+1 < iv.sx and iv.rooms[x+1][y] == "U":
            remaining.append((x+1, y))

        if y-1 >= 0 and iv.rooms[x][y-1] == "R":
            remaining.append((x, y-1))

        if y+1 < iv.sy and iv.rooms[x][y+1] == "L":
            remaining.append((x, y+1))

    return count


if __name__ == "__main__":
    assert len(sys.argv) > 1, "Please specify a filename"
    iv = parse_file(sys.argv[1])
    valid_boundary = find_boundary_valid_rooms(iv)
    valid = [get_valid_room_count_by_room(
        x, y, iv) for (x, y) in valid_boundary]
    # the solution is all the rooms minus the valid rooms.
    print(iv.get_total() - sum(valid))
