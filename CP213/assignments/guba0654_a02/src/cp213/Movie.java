package cp213;

import java.io.PrintStream;

/**
 * Movie class definition.
 *
 * @author Sebastian Gubacsi, 169070654, guba0654@mylaurier.ca
 * @version 2025-10-18
 */
public class Movie implements Comparable<Movie> {

    // Constants
    /**
     * The first year movies were produced.
     */
    public static final int FIRST_YEAR = 1888;
    /**
     * The names of movie genres.
     */
    public static final String[] GENRES = { "science fiction", "fantasy", "drama", "romance", "comedy", "zombie",
	    "action", "historical", "horror", "war", "mystery" };
    /**
     * The maximum allowed {@code Movie} rating.
     */
    public static final double MAX_RATING = 10.0;
    /**
     * The minimum allowed {@code Movie} rating.
     */
    public static final double MIN_RATING = 0.0;

    /**
     * Creates a string of {@code Movie} genres in the format:
     *
     * <pre>
     0: science fiction
     1: fantasy
     2: drama
    ...
    10: mystery
     * </pre>
     *
     * Note the alignment of the genre numbers.
     *
     * @return a formatted numbered string of Movie genres.
     */
    public static String genresMenu() {

	    // your code here
        StringBuilder menu = new StringBuilder();
        for (int i = 0; i < GENRES.length; i++) {
            menu.append(i).append(": ").append(GENRES[i]).append("\n");
        }
        return menu.toString();
    }

    // Attributes
    private String director = "";
    private int genre = -1;
    private double rating = 0;
    private String title = "";
    private int year = 0;

    /**
     * {@code Movie} constructor.
     *
     * @param title    {@code Movie} title
     * @param year     year of release
     * @param director name of director
     * @param rating   rating of 1 - 10 from IMDB
     * @param genre    number representing {@code Movie} genre
     */
    public Movie(final String title, final int year, final String director, final double rating, final int genre) {

	    // your code here
        this.title = title;
        this.year = year;
        this.director = director;
        this.rating = rating;
        this.genre = genre;
    }

    /**
     * {@code Movie} copy constructor. All attributes are primitives or immutable.
     *
     * @param source the {@code Movie} object to copy
     */
    public Movie(Movie source) {

        // your code here
        this.title = source.title;
        this.year = source.year;
        this.director = source.director;
        this.rating = source.rating;
        this.genre = source.genre;
    }

    /**
     * Movies are compared by title, then by year if the titles match. Must ignore
     * case. Ex: {Zulu, 1964} precedes {Zulu, 2013}. Returns:
     * <ul>
     * <li>0 if {@code this} equals {@code target}</li>
     * <li>&lt; 0 if {@code this} precedes {@code target}</li>
     * <li>&gt; 0 if {@code this} follows {@code target}</li>
     * </ul>
     */
    /*
     * (non-Javadoc)
     *
     * @see java.lang.Comparable#compareTo(java.lang.Object)
     */
    @Override
    public int compareTo(final Movie target) {

        // your code here
        int result = this.title.compareToIgnoreCase(target.title);
        if (result == 0) {
            return Integer.compare(this.year, target.year);
        }
        return result;
    }

    /**
     * Converts a genre integer to a string.
     *
     * @return the string version of the genre number
     */
    public String genreToName() {

        // your code here
        return GENRES[this.genre];
    }

    /**
     * Director getter.
     *
     * @return the director
     */
    public String getDirector() {

        // your code here
        return this.director;
    }

    /**
     * Genre getter.
     *
     * @return the genre number
     */
    public int getGenre() {

        // your code here
        return this.genre;
    }

    /**
     * Rating getter.
     *
     * @return the rating
     */
    public double getRating() {

        // your code here
        return this.rating;
    }

    /**
     * Title getter.
     *
     * @return the title
     */
    public String getTitle() {

        // your code here
        return this.title;
    }

    /**
     * Year getter.
     *
     * @return the year
     */
    public int getYear() {

        // your code here
        return this.year;
    }

    /**
     * Creates a formatted string of Movie key data in the format
     *
     * <pre>
     * "title, year"</pre
     * Ex:
     * <pre>
     *
     * "Zulu, 1964"
     * </pre>
     *
     * @return a {@code Movie} key as a string
     */
    public String key() {
	return String.format("%s, %d", this.title, this.year);
    }

    /**
     * Rating setter.
     *
     * @param rating the new rating.
     */
    public void setRating(final double rating) {

        // your code here
        this.rating = rating;

    }

    /**
     * Returns a string in the format:
     *
     * <pre>
    Title:    title
    Year:     year
    Director: director
    Rating:   rating
    Genre:    genre
     * </pre>
     *
     * Note that the field contents are aligned.
     */
    /*
     * (non-Javadoc)
     *
     * @see java.lang.Object#toString() Creates a formatted string of movie data.
     */
    @Override
    public String toString() {

        // your code here
        return String.format("Title:    %s\nYear:     %d\nDirector: %s\nRating:   %.1f\nGenre:    %s\n", this.title, this.year, this.director, this.rating, this.genreToName());
    }

    /**
     * Writes a single line of movie data to an open PrintStream in the format:
     *
     * <pre>
     * "title|year|director|rating|genre"
     * </pre>
     *
     * The quote character (") is not part of the string. See the file movies.txt
     * for an example of the file format and contents.
     *
     * @param ps output {@code PrintStream}
     */
    public void write(final PrintStream ps) {

        // your code here
        ps.printf("%s|%d|%s|%.1f|%d\n", this.title, this.year, this.director, this.rating, this.genre);
        return;
    }

}
