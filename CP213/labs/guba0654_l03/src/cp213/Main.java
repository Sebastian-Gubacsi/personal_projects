package cp213;

import java.time.LocalDate;

/**
 * Tests the Student class.
 *
 * @author your name here
 * @version 2022-01-17
 */
public class Main {

    public static void main(String[] args) {
	final String line = "-".repeat(40);
	int id = 123456;
	String surname = "Brown";
	String forename = "David";
	LocalDate birthDate =  LocalDate.parse("1962-10-25");
	Student student = new Student(id, surname, forename, birthDate);
	System.out.println("New Student:");
	System.out.println(student);
	System.out.println(line);
	System.out.println("Test Getters");

	// call getters here

	System.out.println(line);
	System.out.println("Test Setters");

	// call setters here

	System.out.println("Updated Student:");
	System.out.println(student);
	System.out.println(line);
	System.out.println("Test compareTo");

	// create new Students - call comparisons here
	Student student1 = new Student(123456, "Brown", "David", LocalDate.of(1972, 1, 5));
	Student student2 = new Student(111111, "Brown", "Matthew", LocalDate.of(1952, 7, 15));
	Student student3 = new Student(123456, "Brown", "Alice", LocalDate.of(1970, 5, 15));

	System.out.println("Comparing student1 to student2: " + student1.compareTo(student2));
	System.out.println("Comparing student1 to student3: " + student1.compareTo(student3));
	System.out.println("Comparing student2 to student3: " + student2.compareTo(student3));
    }

}
