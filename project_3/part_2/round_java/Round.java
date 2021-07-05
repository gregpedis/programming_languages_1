import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Scanner;

public class Round {
  public static void main(String[] args) {
    // Solve(args[0]);
    SolveMultiple();
  }

  public static InputVals ParseFile(String filename) throws FileNotFoundException {
    File file = new File(filename);
    Scanner scanner = new Scanner(file);
    String[] line1 = scanner.nextLine().split(" ");
    String[] line2 = scanner.nextLine().split(" ");

    Integer townCount = Integer.parseInt(line1[0]);
    Integer carCount = Integer.parseInt(line1[1]);

    ArrayList<Integer> positions = new ArrayList<Integer>();

    for (String pos : line2) {
      positions.add(Integer.parseInt(pos));
    }

    InputVals inputVals = new InputVals(townCount, carCount, positions);
    scanner.close();
    return inputVals;
  }

  public static int CalculateTownDistance(int town, InputVals inputVals) {
    int current = -1;
    int rest = 0;

    for (int i = 1; i < inputVals.TownCount; i++) {
      int currentPosition = (town + i) % inputVals.TownCount;

      if (inputVals.Positions.containsKey(currentPosition)) {
        int currentDistance = (inputVals.TownCount - i) % inputVals.TownCount;
        if (current == -1) {
          current = currentDistance;
          rest -= current;
        }
        rest += currentDistance * inputVals.Positions.get(currentPosition);
      }
    }

    if (current - rest <= 1) {
      return rest + current;
    } else {
      return -1;
    }
  }

  public static void Solve(String filename) {
    try {
      InputVals inputVals = ParseFile(filename);

      Integer townResult = -1;
      Integer distanceResult = -1;

      for (int i = 0; i < inputVals.TownCount; i++) {
        int distance = CalculateTownDistance(i, inputVals);
        if (distanceResult == -1 || (distance >= 0 && distance < distanceResult)) {
          townResult = i;
          distanceResult = distance;
        }
      }

      System.out.format("%d %d\n", distanceResult, townResult);

    } catch (FileNotFoundException e) {
      e.printStackTrace();
    }
  }

  // TESTBED for all the testcases.
  public static void SolveMultiple() {
    String filename_placeholder = "../../testcases/r@@.txt";
    for (int i = 1; i < 7; i++) {
      String filename = filename_placeholder.replace("@@", Integer.toString(i));
      Solve(filename);
    }
  }
}

class InputVals {
  public int TownCount;
  public int CarCount;
  public HashMap<Integer, Integer> Positions;

  public InputVals(int townCount, int carCount, ArrayList<Integer> positions) {
    TownCount = townCount;
    CarCount = carCount;
    InitializePositions(positions);
  }

  private void InitializePositions(ArrayList<Integer> positions) {
    Positions = new HashMap<Integer, Integer>();
    for (Integer pos : positions) {
      if (!Positions.containsKey(pos)) {
        Positions.put(pos, 0);
      }
      Positions.put(pos, Positions.get(pos) + 1);
    }
  }
}
