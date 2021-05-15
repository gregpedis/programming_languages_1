import sys

class InputVals:
    def __init__(self, sx, sy, rooms):
        self.sx = sx
        self.sy = sy
        self.rooms = rooms

    def get_total(self):
        return self.sx * self.sy

def parse_file(fn):
    with open(fn) as f:
        lines = f.readlines()
        dimensions = [int(x) for x in lines[0].split(" ")]
        return InputVals(dimensions[0], dimensions[1], lines[1:])


def find_boundary_valid_rooms(input_vals):
    valid = []

    for i in range(input_vals.sx):
        if input_vals.rooms[i][0] == "L":
            valid.append((i, 0))
        if input_vals.rooms[i][input_vals.sy-1] == "R":
            valid.append((i, input_vals.sy-1))

    for j in range(input_vals.sy):
        if input_vals.rooms[0][j] == "U":
            valid.append((0, j))
        if input_vals.rooms[input_vals.sx-1][j] == "D":
            valid.append((input_vals.sx-1, j))
    return valid


def get_valid_room_count_by_room(x, y, iv):
    count = 1

    if x-1 >= 0 and iv.rooms[x-1][y] == "D":
        count += get_valid_room_count_by_room(x-1, y, iv)
    if x+1 < iv.sx and iv.rooms[x+1][y] == "U":
        count += get_valid_room_count_by_room(x+1, y, iv)

    if y-1 >= 0 and iv.rooms[x][y-1] == "R":
        count += get_valid_room_count_by_room(x, y-1, iv)

    if y+1 < iv.sy and iv.rooms[x][y+1] == "L":
        count += get_valid_room_count_by_room(x, y+1, iv)

    return count

if __name__ == "__main__":
    assert len(sys.argv) > 1, "Please specify a filename"
    iv = parse_file(sys.argv[1])
    valid_boundary = find_boundary_valid_rooms(iv)
    valid = [get_valid_room_count_by_room(x, y, iv) for (x, y) in valid_boundary]
    print(iv.get_total() - sum(valid))
