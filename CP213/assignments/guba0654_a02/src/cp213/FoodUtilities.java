package cp213;

import java.io.PrintStream;
import java.util.ArrayList;
import java.util.Scanner;

/**
 * Utilities for working with Food objects.
 *
 * @author Sebastian Gubacsi, 169070654, guba0654@mylaurier.ca
 * @version 2025-05-04
 */
public class FoodUtilities {

    /**
     * Determines the average calories in a list of foods. No rounding necessary.
     * Foods list parameter may be empty.
     *
     * @param foods a list of Food
     * @return average calories in all Food objects
     */
    public static int averageCalories(final ArrayList<Food> foods) {

        // your code here
        int total = 0;
        int count = 0;
        for (Food f : foods) {
            total += f.getCalories();
            count += 1;
        }

        if (count != 0) {
            return total / count;
        } else {
            return 0;
        }
    }

    /**
     * Determines the average calories in a list of foods for a particular origin.
     * No rounding necessary. Foods list parameter may be empty.
     *
     * @param foods  a list of Food
     * @param origin the origin of the Food
     * @return average calories for all Foods of the specified origin
     */
    public static int averageCaloriesByOrigin(final ArrayList<Food> foods, final int origin) {

        // your code here
        int total = 0;
        int count = 0;
        for (Food f : foods) {
            if (f.getOrigin() == origin) {
                total += f.getCalories();
                count += 1;
            }
        }

        if (count != 0) {
            return total / count;
        } else {
            return 0;
        }
    }

    /**
     * Creates a list of foods by origin.
     *
     * @param foods  a list of Food
     * @param origin a food origin
     * @return a list of Food from origin
     */
    public static ArrayList<Food> getByOrigin(final ArrayList<Food> foods, final int origin) {

        // your code here
        ArrayList<Food> lst = new ArrayList<>();
        for (Food f : foods) {
            if (f.getOrigin() == origin) {
                lst.add(f);
            }
        }

        return lst;
    }

    /**
     * Creates a Food object by requesting data from a user. Uses the format:
     *
     * <pre>
    Name: name
    Origins
     0 Canadian
     1 Chinese
    ...
    11 English
    Origin: origin number
    Vegetarian (Y/N): Y/N
    Calories: calories
     * </pre>
     *
     * @param keyboard a keyboard Scanner
     * @return a Food object
     */
    public static Food getFood(final Scanner keyboard) {

        // your code here
        System.out.print("Name: ");
        String name = keyboard.nextLine();

        System.out.print(Food.originsMenu());

        System.out.print("Origin: ");
        String sorigin = keyboard.nextLine();
        int origin = Integer.parseInt(sorigin);

        System.out.print("Vegetarian (Y/N): ");
        String svegetarian = keyboard.nextLine();
        boolean vegetarian;
        if (svegetarian.equals("Y")) {
            vegetarian = true;
        } else {
            vegetarian = false;
        }

        System.out.print("Calories: ");
        String scalories = keyboard.nextLine();
        int calories = Integer.parseInt(scalories);

        Food usersFood = new Food(name, origin, vegetarian, calories);

        return usersFood;
    }

    /**
     * Creates a list of vegetarian foods.
     *
     * @param foods a list of Food
     * @return a list of vegetarian Food
     */
    public static ArrayList<Food> getVegetarian(final ArrayList<Food> foods) {

        // your code here
        ArrayList<Food> lst = new ArrayList<>();
        for (Food f : foods) {
            if (f.isVegetarian()) {
                lst.add(f);
            }
        }

        return lst;
    }

    /**
     * Creates and returns a Food object from a line of string data.
     *
     * @param line a vertical bar-delimited line of food data in the format
     *             name|origin|isVegetarian|calories
     * @return the data from line as a Food object
     */
    public static Food readFood(final String line) {

        // your code here
        String[] vals = line.split("\\|");

        String name = vals[0];

        String sorigin = vals[1];
        int origin = Integer.parseInt(sorigin);

        String svegetarian = vals[2];
        boolean vegetarian;
        if (svegetarian.equals("true")) {
            vegetarian = true;
        } else {
            vegetarian = false;
        }

        String scalories = vals[3];
        int calories = Integer.parseInt(scalories);

        Food newFood = new Food(name, origin, vegetarian, calories);

        return newFood;
    }

    /**
     * Reads a file of food strings into a list of Food objects.
     *
     * @param fileIn a Scanner of a Food data file in the format
     *               name|origin|isVegetarian|calories
     * @return a list of Food
     */
    public static ArrayList<Food> readFoods(final Scanner fileIn) {

        // your code here
        ArrayList<Food> lst = new ArrayList<>();

        while (fileIn.hasNextLine()) {
            String line = fileIn.nextLine();

            Food newFood = FoodUtilities.readFood(line);

            lst.add(newFood);

        }

        return lst;
    }

    /**
     * Searches for foods that fit certain conditions.
     *
     * @param foods        a list of Food
     * @param origin       the origin of the food; if -1, accept any origin
     * @param maxCalories  the maximum calories for the food; if 0, accept any
     * @param isVegetarian whether the food is vegetarian or not; if false accept
     *                     any
     * @return a list of foods that fit the conditions specified
     */
    public static ArrayList<Food> foodSearch(final ArrayList<Food> foods, final int origin, final int maxCalories,
	    final boolean isVegetarian) {

        // your code here

        ArrayList<Food> lst = new ArrayList<>();
        for (Food f : foods) {
            if ((f.getOrigin() == origin || origin == -1) && (f.getCalories() <= maxCalories || maxCalories == 0) && (f.isVegetarian() || !isVegetarian)) {
                lst.add(f);
            }
        }

        return lst;
    }

    /**
     * Writes the contents of a list of Food to a PrintStream.
     *
     * @param foods a list of Food
     * @param ps    the PrintStream to write to
     */
    public static void writeFoods(final ArrayList<Food> foods, PrintStream ps) {

	    // your code here
        for (Food f : foods) {
            f.write(ps);
        }
    }
}
