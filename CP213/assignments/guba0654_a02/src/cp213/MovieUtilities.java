package cp213;

import java.io.PrintStream;
import java.util.ArrayList;
import java.util.Scanner;

/**
 * Utilities for working with Movie objects.
 *
 * @author Sebastian Gubacsi, 169070654, guba0654@mylaurier.ca
 * @version 2025-10-18
 */
public class MovieUtilities {

    /**
     * Counts the number of movies in each genre given in {@code Movie.GENRES}. An
     * empty movies list should produce a count array of: [0,0,0,0,0,0,0,0,0,0,0]
     *
     * @param movies {@code List} of movies
     * @return number of genres across all movies - one entry for each genre in the
     *         {@code Movie.GENRES} array
     */
    public static int[] genreCounts(final ArrayList<Movie> movies) {

        // your code here

        int[] genreCounts = new int[Movie.GENRES.length];

        for (Movie movie : movies) {
            int genre = movie.getGenre();
            if (genre >= 0 && genre < Movie.GENRES.length) {
            genreCounts[genre]++;
            }
        }

        return genreCounts;
    }

    /**
     * Creates a {@code Movie} object by requesting data from a user. Uses the
     * format:
     *
     * <pre>
    Title:
    Year:
    Director:
    Rating:
    Genres:
     0: science fiction
     1: fantasy
    ...
    10: mystery

    Enter a genre number:
     * </pre>
     *
     * @param keyboard a keyboard ({@code System.in}) {@code Scanner} object
     * @return a {@code Movie} object.
     */
    public static Movie getMovie(final Scanner keyboard) {

        // your code here
        System.out.print("Title: ");
        String title = keyboard.nextLine();

        System.out.print("Year: ");
        int year = keyboard.nextInt();
        keyboard.nextLine(); // Consume the newline

        System.out.print("Director: ");
        String director = keyboard.nextLine();

        System.out.print("Rating: ");
        double rating = keyboard.nextDouble();

        System.out.println("Genres: ");
        System.out.println(Movie.genresMenu());

        System.out.print("Enter a genre number: ");
        int genre = keyboard.nextInt();

        return new Movie(title, year, director, rating, genre);
    }

    /**
     * Creates a {@code List} of movies whose genre is equal to the {@code genre}
     * parameter.
     *
     * @param movies {@code List} of movies
     * @param genre  genre to compare against
     * @return {@code List} of movies of type {@code genre}
     */
    public static ArrayList<Movie> getByGenre(final ArrayList<Movie> movies, final int genre) {

        // your code here
        ArrayList<Movie> result = new ArrayList<>();

        for (Movie movie : movies) {
            if (movie.getGenre() == genre) {
            result.add(movie);
            }
        }

        return result;
    }

    /**
     * Creates a {@code List} of Movies whose ratings are equal to or higher than
     * rating.
     *
     * @param movies {@code List} of movies
     * @param rating rating to compare against
     * @return {@code List} of movies of {@code rating} or higher
     */
    public static ArrayList<Movie> getByRating(final ArrayList<Movie> movies, final double rating) {

        // your code here
        ArrayList<Movie> result = new ArrayList<>();

        for (Movie movie : movies) {
            if (movie.getRating() >= rating) {
            result.add(movie);
            }
        }

        return result;
    }

    /**
     * Creates a {@code List} of Movies from a particular year.
     *
     * @param movies {@code List} of movies
     * @param year   year to compare against
     * @return {@code List} of movies of year
     */
    public static ArrayList<Movie> getByYear(final ArrayList<Movie> movies, final int year) {

        // your code here
        ArrayList<Movie> result = new ArrayList<>();

        for (Movie movie : movies) {
            if (movie.getYear() == year) {
            result.add(movie);
            }
        }

        return result;
    }

    /**
     * Asks a user to select a genre from a list of genres displayed by calling
     * {@code Movie.genresMenu} and returns an integer genre code. The genre must be
     * a valid index to an item in Movie.GENRES.
     *
     * @param keyboard a keyboard ({@code System.in}) {@code Scanner} object
     * @return an integer genre code
     */
    public static int readGenre(final Scanner keyboard) {

        // your code here
        System.out.println(Movie.genresMenu());
        System.out.print("Enter a genre number: ");
        return keyboard.nextInt();
    }

    /**
     * Creates and returns a {@code Movie} object from a line of formatted string
     * data. The data should be in the format:
     *
     * <pre>
     * "title|year|director|rating|genre"
     * </pre>
     *
     * The quote character (") is not part of the string. See the file movies.txt
     * for an example of the file format and contents.
     *
     * @param line A vertical bar-delimited line of movie data
     *
     * @return the data from line as a {@code Movie} object
     */
    public static Movie readMovie(final String line) {

        // your code here
        String[] fields = line.split("\\|");
        String title = fields[0];
        int year = Integer.parseInt(fields[1]);
        String director = fields[2];
        double rating = Double.parseDouble(fields[3]);
        int genre = Integer.parseInt(fields[4]);

        return new Movie(title, year, director, rating, genre);
    }

    /**
     * Reads a {@code List} of Movies from a file. Each line of the file is in the
     * format:
     *
     * <pre>
     * "title|year|director|rating|genre"
     * </pre>
     *
     * The quote character (") is not part of the string. See the file movies.txt
     * for an example of the file format and contents.
     *
     * @param fileIn a {@code Scanner} of a movie data file
     *
     * @return a {@code List} of movies
     */
    public static ArrayList<Movie> readMovies(final Scanner fileIn) {

        // your code here
        ArrayList<Movie> movies = new ArrayList<>();

        while (fileIn.hasNextLine()) {
            String line = fileIn.nextLine();
            Movie movie = readMovie(line);
            movies.add(movie);
        }

        return movies;
    }

    /**
     * Writes the contents of a {@code List} of movies to a {@code PrintStream}.
     *
     * @param movies {@code List} of movies
     * @param ps     output {@code PrintStream}
     */
    public static void writeMovies(final ArrayList<Movie> movies, PrintStream ps) {

        // your code here
        for (Movie movie : movies) {
            movie.write(ps);
        }
    }

}
