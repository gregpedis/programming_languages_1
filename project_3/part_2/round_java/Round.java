import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Scanner;

public class Round {
  public static void main(String[] args) {
    Solve(args[0]);
    //SolveMultiple();
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

  public static int GetDistance(int townFrom, int townTo, int townCount) {
    return townTo - townFrom >= 0 ? townTo - townFrom : townCount - (townFrom - townTo);
  }

  public static TownDistance CalculateTown(InputVals iv) {
    int maxDistance = GetDistance(iv.GetNextPopulatedTown(), 0, iv.TownCount);

    int totalDistance = 0;
    for (Integer pos : iv.Positions.keySet()) {
      totalDistance += GetDistance(pos, 0, iv.TownCount) * iv.Positions.get(pos);
    }

    return new TownDistance(0, maxDistance, totalDistance);
  }

  public static TownDistance CalculateTown(InputVals iv, TownDistance last) {
    int currentTownId = last.TownId + 1;

    int maxDistance =   iv.Positions.containsKey(currentTownId)
    ? GetDistance(iv.GetNextPopulatedTown(), currentTownId, iv.TownCount)
    : last.MaxDistance + 1;

    int currentTownCarCount = iv.Positions.containsKey(currentTownId) ? iv.Positions.get(currentTownId) : 0;
    int totalDistance = last.TotalDistance + iv.CarCount - iv.TownCount * currentTownCarCount;

    return new TownDistance(currentTownId, maxDistance, totalDistance);
  }

  public static void Solve(String filename) {
    try {
      InputVals inputVals = ParseFile(filename);

      TownDistance last = CalculateTown(inputVals);
      TownDistance result = last.IsValid() ? last : null;

      for (int i = 1; i < inputVals.TownCount; i++) {
        TownDistance current = CalculateTown(inputVals, last);
        if (current.IsValid() && (result == null || current.TotalDistance < result.TotalDistance)) {
          result = current;
        }
        last = current;
      }

      System.out.format("%d %d\n", result.TotalDistance, result.TownId);

    } catch (FileNotFoundException e) {
      e.printStackTrace();
    }
  }

  // TESTBED for all the testcases.
  public static void SolveMultiple() {
    String filename_placeholder = "testcases/r@@.txt";
    for (int i = 1; i < 7; i++) {
      String filename = filename_placeholder.replace("@@", Integer.toString(i));
      Solve(filename);
    }
  }
}

class TownDistance {
  public int TownId;
  public int MaxDistance;
  public int TotalDistance;

  public TownDistance(int townId, int maxDistance, int totalDistance) {
    TownId = townId;
    MaxDistance = maxDistance;
    TotalDistance = totalDistance;
  }

  public boolean IsValid() {
    return MaxDistance - (TotalDistance - MaxDistance) <= 1;
  }
}

class InputVals {
  public int TownCount;
  public int CarCount;
  public HashMap<Integer, Integer> Positions;

  private ArrayList<Integer> _populatedTowns;
  private int _currentPopulatedTownIndex;

  public InputVals(int townCount, int carCount, ArrayList<Integer> positions) {
    TownCount = townCount;
    CarCount = carCount;
    InitializePositions(positions);
    InitializePopulatedTowns();
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

  private void InitializePopulatedTowns() {
    _populatedTowns = new ArrayList<Integer>();
    for (int i = 1; i < TownCount; i++) {
      if (Positions.containsKey(i)) {
        _populatedTowns.add(i);
      }
    }

    if (Positions.containsKey(0)) {
      _populatedTowns.add(0);
    }

    _currentPopulatedTownIndex = -1;
  }

  public int GetNextPopulatedTown() {
    _currentPopulatedTownIndex = (_currentPopulatedTownIndex + 1) % _populatedTowns.size();
    return _populatedTowns.get(_currentPopulatedTownIndex);
  }
}
