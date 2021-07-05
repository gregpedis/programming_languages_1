import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.File;
import java.util.Scanner;

public class Round {
  public static void main(String[] args) {
    // Solve(args[0]);
    SolveMultiple();
  }

  public static InputVals ParseFile(String filename) {
    var file = new File(filename);
    var scanner = new Scanner(file);
    var line1 = scanner.nextLine().split(" ");
    var line2 = scanner.nextLine().split(" ");

    var townCount = Integer.parseInt(line1[0]);
    var carCount = Integer.parseInt(line1[1]);

    var positions = new ArrayList<Integer>();
    for (string pos:line2) {
      positions.add(Integer.parseInt(pos));
    }
  
    var inputVals = new InputVals(townCount, carCount, positions);
    return inputVals;
  }

  public static int GetSum(List<Integer> list) {
    int sum = 0;
    for (var entry: list) {
      sum+= entry;
    }
    return sum;
  }

  public static boolean DistancesAreValid(LinkedList<Integer> distances, int last) {
    var current = distances.pop();
    var diff = current - (GetSum(distances) - last);
    if (diff > 1) {
      return false;
    } else if (diff < -1) {
      return DistancesAreValid(distances, current);
    } else {
      return true;
    }
  }

  public static int CalculateTownDistance(int town, InputVals inputVals) {
    var distances = new LinkedList<Integer>();
    for (int i = 0; i < inputVals.TownCount; i++) {
      var currentPosition = (town + i) % inputVals.TownCount;
      var currentDistance = (inputVals.TownCount - i) % inputVals.TownCount;
      if (inputVals.Positions.containsKey(currentPosition)) {
        for (int j = 0; j < inputVals.Positions.get(currentPosition); j++) {
          distances.push(currentDistance);
        }
      }
    }
    var distanceSum = GetSum(distances);
    return DistancesAreValid(distances, 0) ? distanceSum : -1;
  }

  public static ArrayList<TownDistance> CalculateTownDistances(InputVals inputVals) {
    var distances = new ArrayList<TownDistance>();

    for (int i = 0; i < inputVals.TownCount; i++) {
      var distance = CalculateTownDistance(i, inputVals);
      if (distance >= 0) {
        distances.add(new TownDistance(i, distance));
      }
    }
    return distances;
  }

  public static void Solve(String filename) {
    var inputVals = ParseFile(filename);
    var distances = CalculateTownDistances(inputVals);
    var result = distances.get(0);

    for (var townDistance : distances) {
      if (townDistance.Distance < result.Distance) {
        result = townDistance;
      }
    }
    System.out.format("%d %d", result.Distance, result.Town);
  }

  // TESTBED for all the testcases.
  public static void SolveMultiple() {
    String filename_placeholder = "../testcases/r@@.txt";
    for (int i = 1; i < 3; i++) {
      var filename = filename_placeholder.replace("@@", Integer.toString(i));
      Solve(filename);
    }
  }
}

class TownDistance {
  public int Town;
  public int Distance;

  public TownDistance(int town, int distance) {
    Town = town;
    Distance = distance;
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
    for (var pos : positions) {
      if (!Positions.containsKey(pos)) {
        Positions.put(pos, 0);
      }
      Positions.put(pos, Positions.get(pos) + 1);
    }
  }

}
