from collections import deque as Stack
from collections import deque as Queue

# STACK : append, pop, clear, copy, used as iterable
# QUEUE : append, popleft, clear, copy, used as iterable

filename = "../testcases/qs1.txt"


class State:
    def __init__(self, queue: Queue = None, stack: Stack = None):
        self.queue = queue if queue is not None else Queue()
        self.stack = stack if stack is not None else Stack()
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
        new_state = State(new_q, new_s)
        return new_state

    def execute_operation_S(self):
        new_q = self.queue.copy()
        new_s = self.stack.copy()
        v = new_s.pop()
        new_q.append(v)
        new_state = State(new_q, new_s)
        return new_state

    def __hash__(self):
        return hash((tuple(self.queue), tuple(self.stack)))

    def __eq__(self, other):
        same_q = self.queue == other.queue
        same_s = self.stack == other.stack
        return same_q and same_s

    def __ne__(self, other):
        return not self == other


class TreeNode:
    def __init__(self, state: State, ops_string: str, left=None, right=None):
        self.state = state
        self.ops_string = ops_string
        self.child_left = left
        self.child_right = right


def parse_file(filename):
    with open(filename, "rt") as f:
        lines = f.readlines()
        return [int(x) for x in lines[1].split(" ")]


def generate_children(parent: TreeNode, visited: set):
    if parent is None or parent.state.done:
        return

    if parent.state.operation_Q_allowed():
        left_state = parent.state.execute_operation_Q()
        if left_state not in visited:
            parent.child_left = TreeNode(
                left_state, parent.ops_string + "Q")
            if not left_state.done:
                visited.add(left_state)

    if parent.state.operation_S_allowed():
        right_state = parent.state.execute_operation_S()
        if right_state not in visited:
            parent.child_right = TreeNode(
                right_state, parent.ops_string + "S")
            if not right_state.done:
                visited.add(right_state)

    generate_children(parent.child_left, visited)
    generate_children(parent.child_right, visited)


def create_tree(values):
    state = State(Queue(values))

    parent = TreeNode(state, "")
    visited = {state}
    generate_children(parent, visited)

    return parent


def bfs(node: TreeNode):
    return ""


def solve():
    values = parse_file(filename)
    tree = create_tree(values)
    return tree
    solution = bfs(tree)
    return solution


if __name__ == "__main__":
    solution = solve()
    print(solution)
