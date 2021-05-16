import sys 
# This is redundant, but it makes the program more readable,
# and that's what python is all about!
from collections import deque as Stack
from collections import deque as Queue

# STACK : append, pop, copy
# QUEUE : append, popleft, copy

class State:
    def __init__(self, ops_string="", queue: Queue = None, stack: Stack = None):
        # the current queue state
        self.queue = queue if queue is not None else Queue()
        # the current stack state
        self.stack = stack if stack is not None else Stack()
        # the operations that were applied from the original state to get here.
        self.ops_string = ops_string
        # calculates if the queue is sorted and the stack is empty.
        self.done = self._is_done()

    def _is_done(self):
        q_sorted = all(self.queue[i] <= self.queue[i+1]
                       for i in range(len(self.queue)-1))
        return q_sorted and len(self.stack) == 0

    def operation_Q_allowed(self):
        return len(self.queue) > 0

    def operation_S_allowed(self):
        return len(self.stack) > 0

    # executes a transition to a new state by doing a single Q operation
    def execute_operation_Q(self):
        new_q = self.queue.copy()
        new_s = self.stack.copy()
        v = new_q.popleft()
        new_s.append(v)
        new_state = State(self.ops_string + "Q", new_q, new_s)
        return new_state

    # executes a transition to a new state by doing a single S operation
    def execute_operation_S(self):
        new_q = self.queue.copy()
        new_s = self.stack.copy()
        v = new_s.pop()
        new_q.append(v)
        new_state = State(self.ops_string + "S", new_q, new_s)
        return new_state

    # Make this class hashable, in order to add it on sets.
    # First, make the Q and S into tuples [which are immutable, hashable types]
    # Then hash a tuple of the tuple Q and tuple S, which is immutable and hashable.
    def __hash__(self):
        return hash((tuple(self.queue), tuple(self.stack)))

    # This might be redundant.
    def __eq__(self, other):
        same_q = self.queue == other.queue
        same_s = self.stack == other.stack
        return same_q and same_s

    # This also might be redundant.
    def __ne__(self, other):
        return not self == other


# the file operations
def parse_file(filename):
    with open(filename, "rt") as f:
        lines = f.readlines()
        # list comprehension to read the values
        values = [int(x) for x in lines[1].split(" ")]
        # initialize the original state.
        return State("", Queue(values))



# bfs without creating the tree.
# since it's not necessary and this is faster.
# minor help from https://www.programiz.com/dsa/graph-bfs
# just to remind me of the queue BFS implementation.
def bfs(initial: State):
    visited = {initial}
    state_queue = Queue([initial])

    # while thingies exist in the queue of states, keep going.
    while state_queue:
        curr_state = state_queue.popleft()
        # short-circuit the bfs is a solution is found.
        # returns the steps to reach the solution in a string.
        if curr_state.done:
            return curr_state.ops_string

        # tries to execute a Q operation
        if curr_state.operation_Q_allowed():
            next_state_q = curr_state.execute_operation_Q()
            if next_state_q not in visited:
                visited.add(next_state_q)
                state_queue.append(next_state_q)

        # tries to execute an S operation
        if curr_state.operation_S_allowed():
            next_state_s = curr_state.execute_operation_S()
            if next_state_s not in visited:
                visited.add(next_state_s)
                state_queue.append(next_state_s)

    raise Exception("BFS: No solution found.")



# the solver.
def solve(filename):
    # parses the file and initializes the first state.
    initial_state = parse_file(filename)
    # executes a bfs search of a solution.
    solution = bfs(initial_state)
    # the result.
    print(solution if solution else "empty")


# testbed.
def solve_multiple():
    filename_placeholder = "../testcases/qs@@.txt"
    fns = [filename_placeholder.replace("@@", str(i)) 
            for i in range(1, 6)]
    for fn in fns:
        solve(fn)


if __name__ == "__main__":
    #solve(sys.argv[1])
    solve_multiple()
