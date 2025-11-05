package cp213;

/**
 * @author Your name and id here
 * @version 2025-09-07
 */
public class Strings {
    // Constants
    public static final String VOWELS = "aeiouAEIOU";

    /**
     * Determines if {@code string} is a "palindrome": a word, verse, or sentence
     * (such as "Able was I ere I saw Elba") that reads the same backward or
     * forward. Ignores case, spaces, digits, and punctuation in {@code string}.
     *
     * @param string a string
     * @return {@code true} if {@code string} is a palindrome, {@code false}
     *         otherwise
     */
    public static boolean isPalindrome(final String string) {

        // your code here
        String clean = "";
        for (int i = 0; i < string.length(); i++) {
            if (Character.isLetter(string.charAt(i))) {
                clean += string.charAt(i);
            }
        }
        clean = clean.toLowerCase();
        String reverse = "";
        for (int i = clean.length() - 1; i >= 0; i--) {
            reverse += clean.charAt(i);
        }
        return clean.equals(reverse);
    }

    /**
     * Determines if {@code name} is a valid Java variable name. Variables names
     * must start with a letter or an underscore, but cannot be an underscore alone.
     * The rest of the variable name may consist of letters, numbers and
     * underscores.
     *
     * @param name a string to test as a Java variable name
     * @return {@code true} if {@code name} is a valid Java variable name,
     *         {@code false} otherwise
     */
    public static boolean isValid(final String name) {

        // your code here
        Boolean valid = true;
        if (name.length() == 0 || (!Character.isLetter(name.charAt(0))
                && !(name.charAt(0) == '_') || name.length() == 1 && name.charAt(0) == '_')) {
            valid = false;
        } else {
            for (int i = 1; i < name.length(); i++) {
                if (!Character.isLetterOrDigit(name.charAt(i)) && !(name.charAt(i) == '_')) {
                    valid = false;
                }
            }
        }
        return valid;
    }

    /**
     * Converts a word to Pig Latin. The conversion is:
     * <ul>
     * <li>if a word begins with a vowel, add "way" to the end of the word.</li>
     * <li>if the word begins with consonants, move the leading consonants to the
     * end of the word and add "ay" to the end of that. "y" is treated as a
     * consonant if it is the first character in the word, and as a vowel for
     * anywhere else in the word.</li>
     * </ul>
     * Preserve the case of the word - i.e. if the first character of word is
     * upper-case, then the new first character should also be upper case.
     *
     * @param word the string to convert to Pig Latin
     * @return the Pig Latin version of word
     */
    public static String pigLatin(String word) {

        // your code here
        String pig = "";
        int firstVowel = -1;

        for (int i = 0; i < word.length(); i++) {
            if (VOWELS.indexOf(word.charAt(i)) != -1 || (word.charAt(i) == 'y' && i != 0)) {
                firstVowel = i;
                break;
            }
        }
        if (firstVowel == 0) {
            pig = word + "way";
        } else if (firstVowel > 0) {
            pig = word.substring(firstVowel) + word.substring(0, firstVowel) + "ay";
            if (Character.isUpperCase(word.charAt(0))) {
                pig = Character.toUpperCase(pig.charAt(0)) + pig.substring(1).toLowerCase();
            } else {
                pig = pig.toLowerCase();
            }
        } else {
            pig = word;
        }
        return pig;
    }

}
