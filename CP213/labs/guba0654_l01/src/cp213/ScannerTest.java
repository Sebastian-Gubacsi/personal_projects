package cp213;

import java.util.Scanner;

/**
 * Class to demonstrate the use of Scanner with a keyboard and File objects.
 *
 * @author sebastian gubacsi
 * @version 2022-01-08
 */
public class ScannerTest {

    /**
     * Count lines in the scanned file.
     *
     * @param source Scanner to process
     * @return number of lines in scanned file
     */
    public static int countLines(final Scanner source) {
	int count = 0;

	// your code here
    while (source.hasNextLine()) {
        source.nextLine();
        count++;
    }

	return count;
    }

    /**
     * Count tokens in the scanned object.
     *
     * @param source Scanner to process
     * @return number of tokens in scanned object
     */
    public static int countTokens(final Scanner source) {
	int tokens = 0;

	// your code here
    while (source.hasNext()) {
        source.next();
        tokens++;
    }

	return tokens;
    }

    /**
     * Ask for and total integers from the keyboard.
     *
     * @param source Scanner to process
     * @return total of positive integers entered from keyboard
     */
    public static int readNumbers(final Scanner keyboard) {
	int total = 0;

	// your code here
    while (true) {
	    if (keyboard.hasNextInt()) {
		int number = keyboard.nextInt();
		total += number;
	    }

	    else {
		String input = keyboard.next();
		if (input.equals("q")) {
		    break;
		} else {
		    System.out.println("'" + input + "' is not an integer or 'q'.");
		}
	    }

	}

	return total;
    }


}
