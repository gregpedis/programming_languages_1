import sys 
from collections import deque as Stack
from collections import deque as Queue

# STACK : append, pop, clear, copy, used as iterable
# QUEUE : append, popleft, clear, copy, used as iterable

filename_placeholder = "../testcases/qs@@.txt"


class State:
    def __init__(self, ops_string="", queue: Queue = None, stack: Stack = None):
        self.queue = queue if queue is not None else Queue()
        self.stack = stack if stack is not None else Stack()
        self.ops_string = ops_string
        self.done = self._is_done()

    def _is_done(self):
        q_sorted = all(self.queue[i] <= self.queue[i+1]
                       for i in range(len(self.queue)-1))
        return q_sorted and len(self.stack) == 0

    def operation_Q_allowed(self):
        return len(self.queue) > 0

    def operation_S_allowed(self):
        return len(self.stack) > 0

    def execute_operation_Q(self):
        new_q = self.queue.copy()
        new_s = self.stack.copy()
        v = new_q.popleft()
        new_s.append(v)
        new_state = State(self.ops_string + "Q", new_q, new_s)
        return new_state

    def execute_operation_S(self):
        new_q = self.queue.copy()
        new_s = self.stack.copy()
        v = new_s.pop()
        new_q.append(v)
        new_state = State(self.ops_string + "S", new_q, new_s)
        return new_state

    def __hash__(self):
        return hash((tuple(self.queue), tuple(self.stack)))

    def __eq__(self, other):
        same_q = self.queue == other.queue
        same_s = self.stack == other.stack
        return same_q and same_s

    def __ne__(self, other):
        return not self == other


def parse_file(filename):
    with open(filename, "rt") as f:
        lines = f.readlines()
        values = [int(x) for x in lines[1].split(" ")]
        return State("", Queue(values))


def bfs(initial: State):
    visited = {initial}
    state_queue = Queue([initial])

    while state_queue:
        curr_state = state_queue.popleft()
        if curr_state.done:
            return curr_state.ops_string

        if curr_state.operation_Q_allowed():
            next_state_q = curr_state.execute_operation_Q()
            if next_state_q not in visited:
                visited.add(next_state_q)
                state_queue.append(next_state_q)

        if curr_state.operation_S_allowed():
            next_state_s = curr_state.execute_operation_S()
            if next_state_s not in visited:
                visited.add(next_state_s)
                state_queue.append(next_state_s)

    raise Exception("BFS: No solution found.")


def solve(filename):
    initial_state = parse_file(filename)
    solution = bfs(initial_state)
    print(solution if solution else "empty")


def solve_multiple():
    fns = [filename_placeholder.replace("@@", str(i)) 
            for i in range(1, 6)]
    for fn in fns:
        solve(fn)


if __name__ == "__main__":
    #solve(sys.argv[1])
    solve_multiple()
