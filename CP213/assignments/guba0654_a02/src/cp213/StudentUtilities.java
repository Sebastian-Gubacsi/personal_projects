package cp213;

import java.io.FileNotFoundException;
import java.io.PrintStream;
import java.time.LocalDate;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Scanner;

/**
 * Utilities for working with Student objects.
 *
 * @author David Brown
 * @version 2025-05-04
 */
public class StudentUtilities {

    /**
     * Creates a list of Students whose birthDates are equal to or later than
     * birthDate.
     *
     * @param students  list of students
     * @param birthDate to compare against
     * @return list of students for birthDate
     */
    public static ArrayList<Student> getByBirthDate(final ArrayList<Student> students, final LocalDate birthDate) {
	final ArrayList<Student> rStudents = new ArrayList<>();

	for (final Student student : students) {
	    if (student.getBirthDate().equals(birthDate) || student.getBirthDate().isAfter(birthDate)) {
		rStudents.add(student);
	    }
	}
	return rStudents;
    }

    /**
     * Creates a list of Students whose list of majors include major.
     *
     * @param students list of students
     * @param major    major to compare against
     * @return list of students for major
     */
    public static ArrayList<Student> getByMajor(final ArrayList<Student> students, final int major) {
	final ArrayList<Student> gStudents = new ArrayList<>();

	for (final Student student : students) {
	    for (int g : student.getMajors())
		if (g == major) {
		    gStudents.add(student);
		    break;
		}
	}
	return gStudents;
    }

    /**
     * Creates a list of Students whose list of majors include all the major codes
     * in majors.
     *
     * @param students list of students
     * @param majors   majors list to compare against
     * @return list of students for majors
     */
    public static ArrayList<Student> getByMajors(final ArrayList<Student> students, final Integer[] majors) {
	final ArrayList<Student> gStudents = new ArrayList<>();

	for (final Student student : students) {

	    if (Arrays.equals(student.getMajors(), majors))
		gStudents.add(student);
	}
	return gStudents;
    }

    /**
     * Creates a list of Students from a particular birth year.
     *
     * @param students list of students
     * @param year     birth date year of students
     * @return list of students for birthDate
     */
    public static ArrayList<Student> getByYear(final ArrayList<Student> students, final int year) {
	final ArrayList<Student> yStudents = new ArrayList<>();

	for (final Student student : students) {
	    if (student.getBirthDate().getYear() == year) {
		yStudents.add(student);
	    }
	}
	return yStudents;
    }

    /**
     * Creates a Student object by requesting data from a user.
     *
     * @param keyboard a keyboard Scanner
     * @return a Student object
     */
    public static Student getStudent(final Scanner keyboard) {
	System.out.print("ID: ");
	final int id = keyboard.nextInt();
	keyboard.nextLine();
	System.out.print("Surname: ");
	final String surname = keyboard.nextLine();
	System.out.print("Forename: ");
	final String forename = keyboard.nextLine();
	System.out.print("BirthDate: ");
	final LocalDate birthDate = LocalDate.parse(keyboard.nextLine());
	System.out.print("Majors: ");
	final Integer[] majors = readMajors(keyboard);
	final Student student = new Student(id, surname, forename, birthDate, majors);
	return student;
    }

    /**
     * Counts the number of students in each major given in Student.GENRES.
     *
     * @param students list of students
     * @return Number of majors across all Students.
     */
    public static int[] majorCounts(final ArrayList<Student> students) {
	final int[] counts = new int[Student.MAJORS.length];

	for (Student student : students) {
	    for (int major : student.getMajors()) {
		counts[major] += 1;
	    }
	}
	return counts;
    }

    /**
     * Asks a user to select majors from a list of majors and returns an integer
     * list of the majors chosen.
     *
     * @param keyboard Keyboard input.
     * @return An integer list of major codes.
     */
    public static Integer[] readMajors(final Scanner keyboard) {
	final ArrayList<Integer> majors = new ArrayList<>();
	final int n = Student.MAJORS.length;
	int major = 0;
	System.out.println(Student.majorsMenu());
	System.out.print("Enter a major number ('q' to quit): ");

	while (majors.size() == 0 || !keyboard.hasNext("q")) {

	    if (keyboard.hasNextInt()) {
		major = keyboard.nextInt();

		if (major < 0) {
		    System.out.println("Major code must be greater than 0");
		} else if (major >= n) {
		    System.out.println("Major code must be less than " + n);
		} else if (majors.contains(major)) {
		    System.out.println("Major code already in list");
		} else {
		    majors.add(major);
		}
	    } else if (majors.size() == 0) {
		System.out.println("No valid majors entered yet");
	    } else {
		System.out.println("Not a number");
	    }
	    keyboard.nextLine();
	    System.out.print("Enter a major number ('q' to quit): ");
	}
	keyboard.next();
	Collections.sort(majors);
	return majors.toArray(new Integer[1]);
    }

    /**
     * Creates and returns a Student object from a line of formatted string data.
     *
     * @param line a vertical bar-delimited line of student data in the format
     *             id|surname|forename|birthDate|majorCodes
     * @return the data from line as a Student object
     */
    public static Student readStudent(final String line) {
	final Scanner lineScan = new Scanner(line);
	lineScan.useDelimiter("\\|");
	final int id = lineScan.nextInt();
	final String surname = lineScan.next();
	final String forename = lineScan.next();
	final LocalDate birthDate = LocalDate.parse(lineScan.next());
	final String majorsString = lineScan.next();
	lineScan.close();
	final Integer[] majors = Student.majorsStringToList(majorsString);
	final Student student = new Student(id, surname, forename, birthDate, majors);
	return student;
    }

    /**
     * Reads a list of Students from a file.
     *
     * @param fileScanner The file to read.
     * @return A list of Student objects.
     * @throws FileNotFoundException file not found
     */
    public static ArrayList<Student> readStudents(final Scanner fileScanner) throws FileNotFoundException {
	final ArrayList<Student> students = new ArrayList<>();
	Student student = null;

	while (fileScanner.hasNextLine()) {
	    student = readStudent(fileScanner.nextLine());
	    students.add(student);
	}
	fileScanner.close();
	return students;
    }

    /**
     * Writes the contents of students to file. Overwrites or creates a new file of
     * Student objects converted to strings.
     *
     * @param students List of Students.
     * @param ps       PrintStream to write Student data to.
     */
    public static void writeStudents(final ArrayList<Student> students, PrintStream ps) {

	for (final Student student : students) {
	    student.write(ps);
	}
	return;
    }

}
