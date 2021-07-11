import java.util.Stack; // stack
import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList; // list
import java.util.LinkedList; // queue
import java.util.List;
import java.util.Scanner;
import java.util.HashSet;

public class QSsort {
  public static void main(String[] args) {
    SolveMultiple();
   //  Solve(args[0]);
  }

  public static void Solve(String filename) {
    try {
      State initial_state = ParseFile(filename);
      String solution = ExecuteBFS(initial_state);
      System.out.println(solution);
    } catch (FileNotFoundException e) {
      e.printStackTrace();
    }
  }

  // TESTBED for all the testcases.
  public static void SolveMultiple() {
    String filename_placeholder = "project_3/testcases/qs@@.txt";
    for (int i = 1; i < 7; i++) {
      String filename = filename_placeholder.replace("@@", Integer.toString(i));
      Solve(filename);
    }
  }

  public static String ExecuteBFS(State initial) {
    HashSet<Integer> visited = new HashSet<Integer>();
    LinkedList<State> stateQueue = new LinkedList<State>();

    visited.add(initial.hashCode());
    stateQueue.add(initial);

    while (stateQueue.size() > 0) {
      State current = stateQueue.remove();

      if (current.IsDone()) {
        return current.GetOperationsString();
      }

      if (current.OperationAllowedQ()) {
        State nextStateQ = current.ExecuteOperationQ();
        if (!visited.contains(nextStateQ.hashCode())) {
          visited.add(nextStateQ.hashCode());
          stateQueue.add(nextStateQ);
        }
      }

      if (current.OperationAllowedS()) {
        State nextStateS = current.ExecuteOperationS();
        if (!visited.contains(nextStateS.hashCode())) {
          visited.add(nextStateS.hashCode());
          stateQueue.add(nextStateS);
        }
      }

      
      }  return "what the fuck";
    
  }

  public static State ParseFile(String filename) throws FileNotFoundException {
    File myfile = new File(filename);
    Scanner scanner = new Scanner(myfile);
    ArrayList<Integer> mylist = new ArrayList<Integer>();

    scanner.nextLine();
    String[] tokens = scanner.nextLine().split(" ");
    for (String token : tokens) {

      int entry = Integer.parseInt(token);
      mylist.add(entry);
    }

    LinkedList<Integer> q = new LinkedList<Integer>(mylist);
    Stack<Integer> s = new Stack<Integer>();

    scanner.close();
    return new State("", q, s);
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

  private boolean isSorted(List<Integer> collection) {
    for (int i = 0; i < collection.size() - 1; i++) {
      if (collection.get(i) > collection.get(i + 1)) {
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
    LinkedList<Integer> new_queue = new LinkedList<Integer>(_queue);
    Stack<Integer> new_stack = (Stack<Integer>) _stack.clone();

    new_stack.push(new_queue.remove());
    String new_operation_string = _operationsString + "Q";

    return new State(new_operation_string, new_queue, new_stack);
  }

  public State ExecuteOperationS() {
    LinkedList<Integer> new_queue = new LinkedList<Integer>(_queue);
    Stack<Integer> new_stack = (Stack<Integer>) _stack.clone();
    new_queue.add(new_stack.pop());
    String new_operation_string = _operationsString + "S";
    return new State(new_operation_string, new_queue, new_stack);
  }

}
