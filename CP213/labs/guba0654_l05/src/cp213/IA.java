package cp213;

/**
 * Inherited class in simple example of inheritance / polymorphism.
 *
 * @author David Brown
 * @version 2022-02-25
 */
public class IA extends Student {

    // your code here
    private String course;
    /**
     * Constructor for IA.
     *
     * @param lastName  Last name of IA.
     * @param firstName First name of IA.
     * @param studentId Student ID of IA.
     * @param course    Course of IA.
     */
    public IA(String lastName, String firstName, String studentId, String course) {
        super(lastName, firstName, studentId);
        this.course = course;
    }
    /**
     * Returns a string representation of an IA.
     *
     * @return String representation of an IA.
     */
    @Override
    public String toString() {
        return String.format("%s\nCourse: %s", super.toString(), this.course);
    }

}
