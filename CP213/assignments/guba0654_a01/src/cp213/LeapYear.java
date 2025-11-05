package cp213;

/**
 * @author Your name and id here
 * @version 2025-09-07
 */
public class LeapYear {

    /**
     * Determines whether or not a year is a leap year.
     * <p>
     * A leap year is defined as: <blockquote>Every year that is exactly divisible
     * by four is a leap year, except for years that are exactly divisible by 100,
     * but these centurial years are leap years if they are exactly divisible by
     * 400. For example, the years 1700, 1800, and 1900 are not leap years, but the
     * years 1600 and 2000 are.
     * <p>
     * Thus 1996, 2000, and 2004 are leap years, but 1899, 1900, and 1901 are not
     * leap years.</blockquote>
     * <p>
     * (United States Naval Observatory)
     *
     * @param year the year to test ({@code int} greater than 0)
     * @return {@code true} if {@code year} is a leap year, {@code false} otherwise.
     */
    public static boolean isLeapYear(final int year) {

        // your code here
        boolean result;

        if (year % 400 == 0) {
            result = true;
        } else if (year % 100 == 0) {
            result = false;
        } else if (year % 4 == 0) {
            result = true;
        } else {
            result = false;
        }

        return result;
    }

}
