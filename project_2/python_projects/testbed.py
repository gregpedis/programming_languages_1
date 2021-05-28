
# 3 3
# LLL
# RRU
# LUD

class InputData:
    def __init__(self, size_x, size_y, rooms) -> None:
        self.size_x = size_x
        self.size_y = size_y
        self.rooms = rooms

    def get_total_size(self):
        return self.size_x * self.size_y


class Position:
    def __init__(self, x, y) -> None:
        self.x = x
        self.y = y

def parse_file(filename) -> InputData:
    with open(filename, "rt") as fd:
        lines = fd.readlines()
        sizes = lines[0].split(" ")
        sx = int(sizes[0])
        sy = int(sizes[1])

        # TODO: read lines and create something like this shit bellow.
        rooms = [["U", "L"], ["R", "L"]]
        return InputData(sx, sy, rooms)


def find_exiting_perimeter_rooms(input_data: InputData) -> list:
    result = []
    # this is for first/last column.
    for i in range(input_data.size_x):
        left_room = input_data.rooms[i][0]
        right_room = input_data.rooms[i][input_data.size_y-1]
        if left_room == "L":
            result.append(Position(i, 0))
        # TODO: Same for right.

    # TODO: same for first/last row.
    for j in range(input_data.size_y):
        pass

    return result


def find_room_count_ending_to_me(pos: Position, input_data: InputData) -> int:
    total_count = 0
    if pos.x < input_data.size_x - 2:
        if input_data.rooms[pos.x+1][pos.y] == "L":
            total_count += 1 + find_room_count_ending_to_me(Position(pos.x+1, pos.y), input_data)

    # TODO: same old for left, right, up

    # input_data.rooms[pos.x-1][pos.y]
    # input_data.rooms[pos.x][pos.y+1]
    # input_data.rooms[pos.x][pos.y-1]

    # pass
    return total_count


def find_valid_rooms_count(valid_boundary: list, input_data: InputData) -> int:
    counts = []
    for room in valid_boundary:
        counts.append(find_room_count_ending_to_me(room, input_data))
    return sum(counts) + len(valid_boundary)


def main():
    input_data = parse_file("myfile.txt")
    boundary_rooms = find_exiting_perimeter_rooms(input_data)
    valid_count = find_valid_rooms_count(boundary_rooms, input_data)
    result = input_data.get_total_size() - valid_count
    # TODO: print result for sagwnas
    print(4)


if __name__ == "__main__":
    main()
