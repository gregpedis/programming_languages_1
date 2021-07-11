import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Scanner;

public class Round {
  public static void main(String[] args) {
    Solve(args[0]);
    // SolveMultiple();
  }

  // Reads the input from a file and creates an InputVals instance.
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

  // Calculates the distance between townFrom and townTo, only moving in one
  // direction.
  public static int GetDistance(int townFrom, int townTo, int townCount) {
    return townTo - townFrom >= 0 ? townTo - townFrom : townCount - (townFrom - townTo);
  }

  // Calculates the TownDistance instance for the town with index 0.
  public static TownDistance CalculateTown(InputVals iv) {
    int maxDistance = GetDistance(iv.GetNextPopulatedTown(), 0, iv.TownCount);

    int totalDistance = 0;
    for (Integer pos : iv.Positions.keySet()) {
      // Since positions is a frequency table, calculate distance times the frequency
      // of the town.
      totalDistance += GetDistance(pos, 0, iv.TownCount) * iv.Positions.get(pos);
    }

    return new TownDistance(0, maxDistance, totalDistance);
  }

  // Calculates the TownDistance instance for town last+1.
  public static TownDistance CalculateTown(InputVals iv, TownDistance last) {
    int currentTownId = last.TownId + 1;

    // If this town has cars, get distance from the next populated town.
    // Else, use last max distance plus one.
    int maxDistance = iv.Positions.containsKey(currentTownId)
        ? GetDistance(iv.GetNextPopulatedTown(), currentTownId, iv.TownCount)
        : last.MaxDistance + 1;

    int currentTownCarCount = iv.Positions.containsKey(currentTownId) ? iv.Positions.get(currentTownId) : 0;
    int totalDistance = last.TotalDistance + iv.CarCount - iv.TownCount * currentTownCarCount;

    return new TownDistance(currentTownId, maxDistance, totalDistance);
  }

  // Solution entry point.
  public static void Solve(String filename) {
    try {
      InputVals inputVals = ParseFile(filename);

      // Calculate the TownDistance with index 0.
      TownDistance last = CalculateTown(inputVals);
      TownDistance result = last.IsValid() ? last : null;

      // Loop through the towns 1 to N-1 and calculate the TownDistance for each.
      // Update gradually the solution.
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

// This class is a container for a town, containing the max and total distance for this town.
class TownDistance {
  public int TownId;
  public int MaxDistance;
  public int TotalDistance;

  public TownDistance(int townId, int maxDistance, int totalDistance) {
    TownId = townId;
    MaxDistance = maxDistance;
    TotalDistance = totalDistance;
  }

  // A town is considered valid if the max distance is at most 1 more than 
  // the sum of the rest of the distances.
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

  // Frequency table to calculate the total distance of a town,
  // while also be able to do [containsKey] and check if a town has cars in O(1).
  private void InitializePositions(ArrayList<Integer> positions) {
    Positions = new HashMap<Integer, Integer>();
    for (Integer pos : positions) {
      if (!Positions.containsKey(pos)) {
        Positions.put(pos, 0);
      }
      Positions.put(pos, Positions.get(pos) + 1);
    }
  }

  // An arraylist that contains all the towns that have cars, which goes 1..N,0
  // Also initialize an index as -1.
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

  // Consumes the arraylist as a sudo-queue, "bumbing" the currentIndex to the next one, in a cyclic manner.
  public int GetNextPopulatedTown() {
    _currentPopulatedTownIndex = (_currentPopulatedTownIndex + 1) % _populatedTowns.size();
    return _populatedTowns.get(_currentPopulatedTownIndex);
  }
}
