import java.util.ArrayList;

public class Testbed {
  public static void main(String[] args) {
    int myInt = 9;
    double myDouble = myInt; // Automatic casting: int to double

    System.out.println(myInt); // Outputs 9
    System.out.println(myDouble); // Outputs 9.0

    var mything = 3;
    System.out.println(mything);

    var someshit = mything > 3 ? true : mything < 2 ? true : false;

    // var some = new ArrayList<Integer>();

    // for (String string : args) {

    // }

    var some = new Something("name");

  }

  public int add2(int x) {
    return x + 2;
  }

  public float add2(int x, int y) {
    return x + 2;
  }

}

class Something {
  private String Name;

  public String getName() {return this.Name;}
  public void setName(String value) { Name = value;}

  public Something(String name) {
    Name = name;
  }

  public Something() {
  }
}