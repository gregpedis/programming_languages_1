import java.util.Stack;
import java.util.LinkedList;
import java.util.List;
import java.util.Objects;

public class QSsort {
  public static void main(String[] args) {
    //Solve(args[0]);
    SolveMultiple();
  }

  public static void Solve(String filename) {
    var initial_state = ParseFile(filename);
    var solution = ExecuteBFS(initial_state);
    System.out.println(solution);
  }

  // TESTBED for all the testcases.
  public static void SolveMultiple() {
    String filename_placeholder = "../testcases/qs@@.txt";
    for (int i = 1; i < 7; i++) {
      var filename = filename_placeholder.replace("@@", Integer.toString(i));
      Solve(filename);
    }
  }

  public static String ExecuteBFS(State initial) {
    // initialize a set of ints
    // initialize a queue of states
    // while-loop over it and do a bfs
    // when a state IsDone() then return the GetOperationsString()
    return "placeholder";
  }

  public static State ParseFile(String filename) {
    //read the file
    // create an initial state
    // return initial state
    return null;
  }
}

class State {
  private String _operationsString;

  private LinkedList<Integer> _queue;
  private Stack<Integer> _stack;

  public State(String operationsString, LinkedList<Integer> queue, Stack<Integer> stack) {
    _operationsString = operationsString;
    _queue = queue;
    _stack = stack;
  }

  public String GetOperationsString() {
    return _operationsString.isEmpty() ? "empty" : _operationsString;
  }

  public boolean IsDone() {
    return _stack.isEmpty() && isSorted(_queue);
  }

  private boolean isSorted(List<Integer> collection) 
  {
    for (int i = 0; i < collection.size() - 1; i++) 
    {
      if (collection.get(i) > collection.get(i + 1)) 
      {
        return false;
      }
    }
    return true;
  }

  public boolean OperationAllowedQ() {
    return !_queue.isEmpty();
  }

  public boolean OperationAllowedS() {
    return !_stack.empty() && (_queue.isEmpty() || _stack.peek() != _queue.peek());
  }

  public State ExecuteOperationQ() {
    var new_queue = new LinkedList<Integer>(_queue);
    var new_stack = (Stack<Integer>) _stack.clone();

    new_stack.push(new_queue.pop());
    var new_operation_string = _operationsString + "Q";

    return new State(new_operation_string, new_queue, new_stack);
  }

  public State ExecuteOperationS() {
    var new_queue = new LinkedList<Integer>(_queue);
    var new_stack = (Stack<Integer>) _stack.clone();
    new_queue.push(new_stack.pop());
    var new_operation_string = _operationsString + "S";
    return new State(new_operation_string, new_queue, new_stack);
  }

}
