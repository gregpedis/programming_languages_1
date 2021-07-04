import sys
import time
from collections import deque as Queue


class InputVals:
    def __init__(self, town_count, car_count, positions):
        self.town_count = town_count
        self.car_count = car_count
        self._initialize_positions(positions)

    def _initialize_positions(self, positions):
        self.positions = {}
        for p in positions:
            self.positions[p] = self.positions.get(p, 0) + 1


# the file operations
def parse_file(filename) -> InputVals:
    with open(filename, "rt") as f:
        lines = f.readlines()
        line1 = lines[0].split(" ")
        town_count = int(line1[0])
        car_count = int(line1[1])
        # list comprehension to read the values
        positions = [int(x) for x in lines[1].split(" ")]
        # initialize the original state.
        return InputVals(town_count, car_count, positions)


def distances_are_valid(distances:Queue, last=0):
    current = distances.pop()
    diff = current - (sum(distances) - last)
    if diff > 1:
        return False
    elif diff < -1:
        return distances_are_valid(distances, current)
    else:
        return True


def calculate_town_distance(town, input_vals: InputVals) -> int:
    distances = Queue()
    for i in range(input_vals.town_count):
        current_position = (town + i) % input_vals.town_count
        current_distance = (input_vals.town_count - i) % input_vals.town_count
        if current_position in input_vals.positions:
            for i in range(input_vals.positions[current_position]):
                distances.appendleft(current_distance)

    distance_sum = sum(distances)
    if distances_are_valid(distances):
        return distance_sum
    else:
        return -1

def calculate_town_distances(input_vals:InputVals):
    for town in range(input_vals.town_count):
        distance = calculate_town_distance(town, input_vals)
        if distance >=0:
            yield (town, distance)


# the solver.
def solve(filename):
    # parses the file and initializes the first state.
    input_vals = parse_file(filename)
    distances = calculate_town_distances(input_vals)
    result_town, result_distance = next(distances)

    for t, d in distances:
        if d < result_distance:
            result_town, result_distance = t,d

    print(f"{result_distance} {result_town}")


# testbed.
def solve_multiple():
    filename_placeholder = "testcases/r@@.txt"
    fns = [filename_placeholder.replace("@@", str(i))
           for i in range(1, 3)]
    for fn in fns:
        tic = time.perf_counter()
        solve(fn)
        toc = time.perf_counter()
        print(f"Took {toc-tic:0.4f} seconds for {fn}.")


if __name__ == "__main__":
    # solve(sys.argv[1])
    solve_multiple()
