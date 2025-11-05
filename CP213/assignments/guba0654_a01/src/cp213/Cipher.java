package cp213;

/**
 * @author Your name and id here
 * @version 2025-09-07
 */
public class Cipher {
    // Constants
    public static final String ALPHA = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    public static final int ALPHA_LENGTH = ALPHA.length();

    /**
     * Encipher a string using a shift cipher. Each letter is replaced by a letter
     * {@code n} letters to the right of the original. Thus for example, all shift
     * values evenly divisible by 26 (the length of the English alphabet) replace a
     * letter with itself. Non-letters are left unchanged.
     *
     * @param string the string to encipher
     * @param n      the number of letters to shift
     * @return the enciphered string in all upper-case
     */
    public static String shift(final String string, final int n) {

        // your code here
        StringBuilder result = new StringBuilder();
        int shiftAmount = n % ALPHA_LENGTH;

        for (char c : string.toUpperCase().toCharArray()) {
            if (Character.isLetter(c)) {
            int originalIndex = ALPHA.indexOf(c);
            int newIndex = (originalIndex + shiftAmount + ALPHA_LENGTH) % ALPHA_LENGTH;
            result.append(ALPHA.charAt(newIndex));
            } else {
            result.append(c);
            }
        }

        return result.toString();
    }

    /**
     * Encipher a string using the letter positions in ciphertext. Each letter is
     * replaced by the letter in the same ordinal position in the ciphertext.
     * Non-letters are left unchanged. Ex:
     *
     * <pre>
    Alphabet:   ABCDEFGHIJKLMNOPQRSTUVWXYZ
    Ciphertext: AVIBROWNZCEFGHJKLMPQSTUXYD
     * </pre>
     *
     * A is replaced by A, B by V, C by I, D by B, E by R, and so on. Non-letters
     * are ignored.
     *
     * @param string     string to encipher
     * @param ciphertext ciphertext alphabet
     * @return the enciphered string in all upper-case
     */
    public static String substitute(final String string, final String ciphertext) {

        // your code here
        StringBuilder result = new StringBuilder();
        
        // Convert input string to upper case
        String upperString = string.toUpperCase();
        
        // Process each character
        for (int i = 0; i < upperString.length(); i++) {
            char ch = upperString.charAt(i);
            
            // Check if character is a letter
            if (Character.isLetter(ch)) {
                // Find position of character in standard alphabet
                int position = ALPHA.indexOf(ch);
                
                // Replace with character at same position in ciphertext
                if (position >= 0 && position < ciphertext.length()) {
                    result.append(ciphertext.charAt(position));
                } else {
                    // If position not found or out of bounds, keep original
                    result.append(ch);
                }
            } else {
                // Non-letter characters remain unchanged
                result.append(ch);
            }
        }
        
        return result.toString();
    }

}
