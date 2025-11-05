package cp213;

import java.io.PrintStream;
import java.time.LocalDate;
import java.util.ArrayList;
import java.util.Scanner;

/**
 * Student class definition.
 *
 * @author David Brown
 * @version 2025-05-04
 */
public class Student implements Comparable<Student> {

    // Constants
    /**
     * List of string descriptions of student majors.
     */
    public static final String[] MAJORS = { "Computer Science", "Psychology", "Chemistry", "Math", "Business", "Music",
	    "English", "Basket Weaving", "Women's Studies", "History", "Archaeology" };

    /**
     * Returns a string of all majors in the Student.MAJORS list. Use for input
     * menus. Formatted as " 3: Math"
     *
     * @return the majors.
     */
    public static String majorsMenu() {
	String string = "";
	final int n = MAJORS.length;

	for (int i = 0; i < n; i++) {
	    string += String.format("%2d: %s%n", i, MAJORS[i]);
	}
	return string;
    }

    /**
     * Converts a string of the form "2,3,6" to an array of Integer objects, [2, 3,
     * 6]. Used when reading Student objects from a file.
     *
     * @param string The string to convert to an array.
     * @return The array version of string.
     */
    public static Integer[] majorsStringToList(final String string) {
	final ArrayList<Integer> majorsList = new ArrayList<>();
	final Scanner majorsScan = new Scanner(string);
	majorsScan.useDelimiter(",");

	while (majorsScan.hasNextInt()) {
	    majorsList.add(majorsScan.nextInt());
	}
	majorsScan.close();
	// Convert arraylist to an array of Integer.
	return majorsList.toArray(new Integer[1]);
    }

    // Attributes
    private LocalDate birthDate = null;
    private String forename = "";
    private int id = 0;
    private Integer[] majors = null;
    private String surname = "";

    /**
     * Instantiates a Student object.
     *
     * @param id        student ID number
     * @param surname   student surname
     * @param forename  name of forename
     * @param birthDate birthDate in 'YYYY-MM-DD' format
     * @param majors    integers representing student majors list
     */
    public Student(final int id, final String surname, final String forename, final LocalDate birthDate,
	    final Integer[] majors) {
	this.id = id;
	this.surname = surname;
	this.forename = forename;
	this.birthDate = birthDate;
	this.majors = majors;
	return;
    }

    /**
     * Converts a majors list of the form [2,3,7] to a string "2,3,7" for writing
     * Student data to a file.
     *
     * @return the majors list string
     */
    private String majorsListToString() {
	String string = "";
	final int n = this.majors.length;
	int i = 0;

	for (i = 0; i < n - 1; i++) {
	    string += "" + this.majors[i] + ",";
	}
	// Append the last major code without a trailing comma.
	string += "" + this.majors[i];
	return string;
    }

    /*
     * (non-Javadoc)
     *
     * @see java.lang.Comparable#compareTo(java.lang.Object)
     */
    @Override
    public int compareTo(final Student target) {
	// Compare surname first
	int result = this.surname.compareTo(target.surname);

	if (result == 0) {
	    // Compare forenames next
	    result = this.forename.compareTo(target.forename);

	    if (result == 0) {
		// Compare IDs
		result = Integer.compare(this.id, target.id);
	    }
	}
	return result;
    }

    /**
     * birthDate getter.
     *
     * @return the birthDate
     */
    public LocalDate getBirthDate() {
	return this.birthDate;
    }

    /**
     * forename getter.
     *
     * @return the forename
     */
    public String getForename() {
	return this.forename;
    }

    /**
     * id getter
     *
     * @return the id
     */
    public int getId() {
	return id;
    }

    /**
     * majors getter.
     *
     * @return the majors list
     */
    public Integer[] getMajors() {
	return this.majors;
    }

    /**
     * surname getter.
     *
     * @return the surname
     */
    public String getSurname() {
	return this.surname;
    }

    /*
     * (non-Javadoc)
     *
     * @see java.lang.Object#hashCode()
     */
    @Override
    public int hashCode() {
	return this.id;
    }

    /**
     * Creates a formatted string of Student key data in the format "surname,
     * forename, id". Ex: "Brown, David, 123456".
     *
     * @return a Student key as a string
     */
    public String key() {
	return String.format("%s, %s, %d", this.surname, this.forename, this.id);
    }

    /**
     * Converts a list of major integers to a string of major names.
     *
     * @return comma delimited string of major names based upon the current student
     *         object's integer majors list. e.g.: [0, 2] returns "science fiction,
     *         drama"
     */
    public String majorsListToNames() {
	String string = "";
	final int n = this.majors.length;
	int i = 0;

	for (i = 0; i < n - 1; i++) {
	    string += Student.MAJORS[this.majors[i]] + ", ";
	}
	// Append the last major name without a trailing comma.
	string += Student.MAJORS[this.majors[i]];
	return string;
    }

    /**
     * forename setter
     *
     * @param forename the new forename
     */
    public void setForename(String forename) {
	this.forename = forename;
    }

    /**
     * majors setter.
     *
     * @param majors the new list of numeric majors
     */
    public void setMajors(final Integer[] majors) {
	this.majors = majors;
    }

    /**
     * surname setter.
     *
     * @param surname the new surname
     */
    public void setSurname(final String surname) {
	this.surname = surname;
    }

    /*
     * (non-Javadoc)
     *
     * @see java.lang.Object#toString() Creates a formatted string of student data.
     */
    @Override
    public String toString() {
	String string = String.format("Name:      %s, %s%n", this.surname, this.forename);
	string += String.format("ID:        %d%n", this.id);
	string += String.format("Birthdate: %s%n", this.birthDate);
	string += String.format("Majors:    %s%n", this.majorsListToNames());
	return string;
    }

    /**
     * Writes a single line of student data to an open file in the format
     * id|surname|forename|birthDate|majorCodes
     *
     * @param ps output PrintStream to print to
     */
    public void write(final PrintStream ps) {
	final String string = String.format("%d|%s|%s|%s|%s", this.id, this.surname, this.forename, this.birthDate,
		this.majorsListToString());
	ps.println(string);
	return;
    }

}
