package cp213;

/**
 * Inherited class in simple example of inheritance / polymorphism.
 *
 * @author
 * @version 2022-02-25
 */
public class CAS extends Professor {

    // your code here
    private String employeeId;
    /**
     * Constructor for CAS.
     *
     * @param lastName   Last name of CAS.
     * @param firstName  First name of CAS.
     * @param department Department of CAS.
     * @param employeeId Employee ID of CAS.
     */
    public CAS(String lastName, String firstName, String department, String employeeId) {
        super(lastName, firstName, department);
        this.employeeId = employeeId;
    }
    /**
     * Returns a string representation of a CAS.
     *
     * @return String representation of a CAS.
     */
    @Override
    public String toString() {
        return String.format("%s\nEmployee ID: %s", super.toString(), this.employeeId);
    }

}
