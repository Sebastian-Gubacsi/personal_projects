package cp213;

import java.io.PrintStream;

/**
 * Food class definition.
 *
 * @author Sebastian Gubacsi, 169070654, guba0654@mylaurier.ca
 * @version 2025-05-04
 */
public class Food implements Comparable<Food> {

    // Constants
    public static final String ORIGINS[] = { "Canadian", "Chinese", "Indian", "Ethiopian", "Mexican", "Greek",
	    "Japanese", "Italian", "Moroccan", "Scottish", "Columbian", "English" };

    /**
     * Creates a string of food origins in the format:
     *
     * <pre>
    Origins
    0 Canadian
    1 Chinese
    ...
    11 English
     * </pre>
     *
     * @return A formatted numbered string of valid food origins.
     */
    public static String originsMenu() {

        // your code here
        String menu = "Origins\n";
            int i = 0;
            String s;
            for (String origin : ORIGINS) {
                s = Integer.toString(i);
                if (s.length() == 1) {
                    menu += " " + s + " " + origin + "\n";
                } else {
                    menu += s + " " + origin + "\n";
                }
                i += 1;
            }
        return menu;
    }

    // Attributes
    private String name = null;
    private int origin = 0;
    private boolean isVegetarian = false;
    private int calories = 0;

    /**
     * Food constructor.
     *
     * @param name         food name
     * @param origin       food origin code
     * @param isVegetarian whether food is vegetarian
     * @param calories     caloric content of food
     */
    public Food(final String name, final int origin, final boolean isVegetarian, final int calories) {

        // your code here
        this.name = name;
        this.origin = origin;
        this.isVegetarian = isVegetarian;
        this.calories = calories;

        return;
    }

    /**
     * Food copy constructor. All attributes are primitives or immutable.
     *
     * @param source the Food object to copy.
     */
    public Food(Food source) {

        // your code here
        this.name = source.name;
        this.origin = source.origin;
        this.isVegetarian = source.isVegetarian;
        this.calories = source.calories;

    }

    /*
     * (non-Javadoc) Compares this food against another food.
     *
     * @see java.lang.Comparable#compareTo(java.lang.Object)
     */
    /**
     * Foods are compared by name, then by origin if the names match. Must ignore
     * case.
     */
    @Override
    public int compareTo(final Food target) {

    	// your code here
        if (this.name.compareToIgnoreCase(target.name) < 0) {
            return -1;
        }
        // this is lower in alpha
        else if (this.name.compareToIgnoreCase(target.name) > 0) {
            return 1;
        }
        // equal strings
        else {
            // this is first in ORIGIN
            if (this.origin < target.origin) {
                return -1;

            }
            // this is later in ORIGIN
            else if (this.origin > target.origin) {
                return 1;
            }
            // both foods are equal
            else {
                return 0;
            }
        }
    }

    /**
     * Getter for calories attribute.
     *
     * @return calories
     */
    public int getCalories() {

        // your code here

        return this.calories;
    }

    /**
     * Getter for name attribute.
     *
     * @return name
     */
    public String getName() {

        // your code here

        return this.name;
    }

    /**
     * Getter for origin attribute.
     *
     * @return origin
     */
    public int getOrigin() {

        // your code here

        return this.origin;
    }

    /**
     * Getter for string version of origin attribute.
     *
     * @return string version of origin
     */
    public String getOriginString() {

        // your code here
        return ORIGINS[this.origin];
    }

    /**
     * Getter for isVegetarian attribute.
     *
     * @return isVegetarian
     */
    public boolean isVegetarian() {

        // your code here

        return this.isVegetarian;
    }

    /*
     * (non-Javadoc)
     *
     * @see java.lang.Object//toString() Creates a formatted string of food data.
     */
    /**
     * Returns a string version of a Food object in the form:
     *
     * <pre>
    Name:       name
    Origin:     origin string
    Vegetarian: true/false
    Calories:   calories
     * </pre>
     */
    @Override
    public String toString() {

	    // your code here
        String bol;
        if (this.isVegetarian) {
            bol = "true";
        } else {
            bol = "false";
        }
        String cals = Integer.toString(this.calories);
        String str = "Name:       " + this.name + "\n" + "Origin:     " + ORIGINS[this.origin] + "\n" + "Vegetarian: "
                + bol + "\n" + "Calories:   " + cals + "\n";

        return str;
    }

    /**
     * Writes a single line of food data to an open PrintStream. The contents of
     * food are written as a string in the format name|origin|isVegetarian|calories
     * to ps.
     *
     * @param ps The PrintStream to write to.
     */
    public void write(final PrintStream ps) {

	    // your code here
        String bol;
        if (this.isVegetarian) {
            bol = "true";
        } else {
            bol = "false";
        }
        String cals = Integer.toString(this.calories);
        String origin = Integer.toString(this.origin);

        ps.println(this.name + "|" + origin + "|" + bol + "|" + cals);

        return;
    }

}
