#include <iostream>
#include <string>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
using namespace std;

// Function to insert employee data into the "salary" table
void insertEmployeeData(sql::Connection* con)
{
    try
    {
        sql::PreparedStatement* pstmt;

        // Accept input from users
        cout << " Enter Employee ID: ";
        int employeeId;
        cin >> employeeId;

        cout << "\n Enter Name: ";
        string name;
        //cin.ignore();  // Ignore newline character in the input buffer
        getline(cin >> ws, name);

        cout << "\n Enter Age: ";
        int age;
        cin >> age;

        cout << "\n Enter Department: ";
        string department;
        //cin.ignore();  // Ignore newline character in the input buffer
        getline(cin >> ws, department);

        cout << "\n Enter Basic Pay: ";
        int basicPay;
        cin >> basicPay;

        cout << "\n Enter Number of Work Days: ";
        int noWorkDays;
        cin >> noWorkDays;

        cout << "\n Enter Number of Leave Days: ";
        int noLeaveDays;
        cin >> noLeaveDays;

        // Prepare the SQL statement with placeholders
        //prepareStatement() method allows you to 
        // execute the same SQL statement repeatedly with different parameter values.
        pstmt = con->prepareStatement("INSERT INTO salary (Employee_id, name, age, department, basic_pay, no_work_days, no_leave_days) "
            "VALUES (?, ?, ?, ?, ?, ?, ?)");

        // Set parameter values for the prepared statement
        pstmt->setInt(1, employeeId);
        pstmt->setString(2, name);
        pstmt->setInt(3, age);
        pstmt->setString(4, department);
        pstmt->setInt(5, basicPay);
        pstmt->setInt(6, noWorkDays);
        pstmt->setInt(7, noLeaveDays);

        // Execute the prepared statement to insert data into the table
        pstmt->executeUpdate();

        cout << "\n Employee data inserted successfully." << endl;

        // Close the prepared statement
        delete pstmt;
    }
    catch (sql::SQLException& e)
    {
        cerr << "SQL Exception: " << e.what() << endl;
    }
}

void retrieveEmployeeData(sql::Connection* con) {
    try {
        // Create a Statement object to execute SQL queries
        std::unique_ptr<sql::Statement> stmt(con->createStatement());

        // Execute a SELECT query to retrieve employee data
        std::unique_ptr<sql::ResultSet> resultSet(stmt->executeQuery("SELECT * FROM salary"));

        string dept;
        cout << "\nEnter the department: ";
        getline(cin >> ws, dept);

        string month1;
        cout << "\nEnter the Month(mm): ";
        getline(cin >> ws, month1);

        string year1;
        cout << "\nEnter the year(yyyy): ";
        getline(cin >> ws, year1);
        cout << endl;

        // Process the result set
        while (resultSet->next()) {

            // Retrieve column values
            int employeeId = resultSet->getInt("Employee_id");
            std::string name = resultSet->getString("name");
            int age = resultSet->getInt("age");
            std::string department = resultSet->getString("department");
            int basicPay = resultSet->getInt("basic_pay");
            int noWorkDays = resultSet->getInt("no_work_days");
            int noLeaveDays = resultSet->getInt("no_leave_days");
            int gross_pay = basicPay * noWorkDays;
            int net_pay = basicPay * (1 - (noLeaveDays / noWorkDays));
            int HRA = basicPay * 0.5;
            int PF = basicPay * 0.12;
            int Allowance = basicPay * 0.6;
            string time1 = resultSet->getString("Entry_date");
            int pos = time1.find("-"); // May show errors if "-" is not found //size_t datatype will be able to handle the returned value if not found 
            string year = time1.substr(0, pos);
            string month = time1.substr(pos + 1, 2);

            if (department == dept && year == year1 && month == month1)
            {
                // Display retrieved information (you can modify this part as needed)
                cout << "Employee ID: " << employeeId << endl;
                cout << "Name: " << name << endl;
                cout << "Age: " << age << endl;
                cout << "Department: " << department << endl;
                cout << "Basic Pay: " << basicPay << endl;
                cout << "Number of Work Days: " << noWorkDays << endl;
                cout << "Number of Leave Days: " << noLeaveDays << endl;
                cout << "Gross Pay: " << gross_pay << endl;
                cout << "Net Pay: " << net_pay << endl;
                cout << "HRA: " << HRA << endl;
                cout << "PF: " << PF << endl;
                cout << "Allowance:" << Allowance << endl;
                cout << "Time:" << time1 << endl;

                cout << "---------------------------" << endl;
            }
        }
    }
    catch (sql::SQLException& e) {
        cerr << "SQL Exception: " << e.what() << endl;
    }
}

int main()
{
    try {

        int ch;
        char ch1;
        // Create a MySQL Connector/C++ driver object
        sql::mysql::MySQL_Driver* driver; //**
        driver = sql::mysql::get_mysql_driver_instance();

        // Establish a connection to the MySQL server
        std::unique_ptr<sql::Connection> con(driver->connect("tcp://127.0.0.1:3306", "root", "Tamil@1234"));
        cout << "Connection has been established to the MySQL server" << endl;

        // Create a Statement object to execute SQL queries
        std::unique_ptr<sql::Statement> stmt(con->createStatement());

        // Specify the database name to be created
        const std::string databaseName = "employee";

        // SQL query to create the database
        std::string createDatabaseQuery = "CREATE DATABASE IF NOT EXISTS " + databaseName;

        // Execute the SQL query
        stmt->execute(createDatabaseQuery);
        std::cout << "Database '" << databaseName << "' already exists or has been created successfully." << std::endl;

        // Set the default database schema for the connection
        con->setSchema(databaseName);
        std::cout << "The \'" + databaseName + "\' databse has been set as the default database schema for the connection" << endl;

        // Execute the SQL query
        stmt->execute("create table if not exists salary("
            "Employee_id int primary key,"
            "name varchar(225),"
            "age int,"
            "department varchar(225),"
            "basic_pay int,"
            "no_work_days int,"
            "no_leave_days int,"
            "Entry_date timestamp default current_timestamp)");
        cout << "Salary table has been created or it already exists" << endl;
        do
        {
            cout << "\nSalary Calculator";
            cout << "\n~~~~~~~~~~~~~~~~~";
            cout << "\nMenu";
            cout << "\n~~~~";
            cout << "\n1. Salary Info Entry";
            cout << "\n2. Process Info";
            /*cout << "\n3. Generate Report";*/
            do
            {
                cout << "\n\nEnter your choice: ";
                cin >> ch;
            } while (ch < 1 || ch > 2);

            if (ch == 1)
            {
                // Call the function to insert employee data
                insertEmployeeData(con.get()); // get() method is used to pass con as a raw pointer
            }
            else if (ch == 2)
            {
                // Call the function to retrieve employee data
                retrieveEmployeeData(con.get());
            }
            cout << "\nDo you want to go back to the main menu?(Y/N)";
            cin >> ch1;
        } while (ch1 == 'Y');
    }
    catch (sql::SQLException& e)
    {
        std::cerr << "SQL Exception: " << e.what() << std::endl;
    }

    return 0;
}