package cp213;

import java.io.PrintStream;
import java.util.Scanner;

/**
 * @author Your name and id here
 * @version 2025-09-07
 */
public class SerialNumber {

    /**
     * Determines if a string contains all digits.
     *
     * @param string the string to test
     * @return {@code true} if {@code string} is all digits, {@code false} otherwise
     */
    public static boolean allDigits(final String string) {

        // your code here
        boolean result = true;

        for (char c : string.toCharArray()) {
            if (!Character.isDigit(c)) {
            result = false;
            }
        }

        return result;
    }

    /**
     * Determines if a string is a good serial number. Good serial numbers are of
     * the form 'SN/nnnn-nnn', where 'n' is a digit.
     *
     * @param sn the serial number to test
     * @return {@code true} if the serial number is valid in form, {@code false}
     *         otherwise.
     */
    public static boolean validSn(final String sn) {

	    // your code here
        // Check if serial number is null or has incorrect length
        // Format: SN/nnnn-nnn = 11 characters total
        if (sn == null || sn.length() != 11) {
            return false;
        }
        
        // Check if it starts with "SN/"
        if (!sn.startsWith("SN/")) {
            return false;
        }
        
        // Check if character at index 7 is a dash
        if (sn.charAt(7) != '-') {
            return false;
        }
        
        // Extract the digit parts: positions 3-6 (4 digits) and 8-10 (3 digits)
        String firstPart = sn.substring(3, 7);  // 4 digits
        String secondPart = sn.substring(8, 11); // 3 digits
        
        // Verify both parts contain only digits
        return allDigits(firstPart) && allDigits(secondPart);
    }

    /**
     * Evaluates serial numbers from a file. Writes valid serial numbers to
     * {@code goodSns}, and invalid serial numbers to {@code badSns}. Each line of
     * {@code fileIn} contains a (possibly valid) serial number.
     *
     * @param fileIn  a file already open for reading
     * @param goodSns a file already open for writing
     * @param badSns  a file already open for writing
     */
    public static void validSnFile(final Scanner fileIn, final PrintStream goodSns, final PrintStream badSns) {

        // your code here
        // Read each line from the input file
        while (fileIn.hasNextLine()) {
            String serialNumber = fileIn.nextLine();
            
            // Validate the serial number and write to appropriate file
            if (validSn(serialNumber)) {
                goodSns.println(serialNumber);
            } else {
                badSns.println(serialNumber);
            }
        }
        
        return;
    }

}
