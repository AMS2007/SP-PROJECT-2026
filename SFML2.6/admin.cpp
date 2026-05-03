/*#include <iostream>
#include <fstream>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <sstream>
#include <SFML/Button.hpp>
#include <SFML/sfmlbutton.hpp>
#include <SFML/EllipseButton.hpp>
#include <SFML/RectButton.hpp>
using namespace sf;
using namespace std;

const int months = 12;
struct Attendance
{
    int id, month, dayspresent, daysabsent;
};

struct Employee
{
    long long id, age, phone, basicsalary;
    string name, password, position;
    float netsalary = 0, tax, bonus, overtimehrs;
    Attendance attendance[months];
} employee[100];

struct Admin
{
    int id;
    string username;
    string password;
} admin[2];

int admincount = 1;
int employee_count = 5;
int searchId;
int deleteID;
Text errormessage;


//forward declarations 
float salarycalc();
void  addEmployee();
void  manageAttendance();
void  updateEmployee();
void  deleteEmployee();


//definitions
float salarycalc()
{
    int id;
    float basicsalary, overtime, overtimerate, bonus;
    float salary = 0;
    float taxfree = 0, salarydeduction;
    int daysAbsent = 0;
    int month;
    int check = 0;
    do
    {
        cout << "Enter employee id:" << endl;
        cin >> id;

        for (int i = 0; i < 100; i++)
        {
            if (id != employee[i].id)
                continue;
            check = 1;
            cout << employee[i].name << endl;
            while (true)
            {
                cout << "Enter month (1-12):";
                cin >> month;
                if (month < 1 || month > 12)
                {
                    cout << "Invalid Month!" << endl;
                    continue;
                }
                else
                    break;
            }
            cout << "Enter basic salary:";
            cin >> basicsalary;
            cout << "Enter over time hours:";
            cin >> overtime;
            cout << "Enter overtime rate:";
            cin >> overtimerate;
            cout << "Enter bonus:";
            cin >> bonus;
            cout << "Enter salary deduction (per day absent):";
            cin >> salarydeduction;

            employee[i].overtimehrs = overtime;
            employee[i].bonus = bonus;
            employee[i].tax = (basicsalary * 0.14f);
            daysAbsent = employee[i].attendance[month - 1].daysabsent;
            taxfree = basicsalary - (basicsalary * 0.14f);
            salary = taxfree + (overtime * overtimerate)
                + bonus - (daysAbsent * salarydeduction);
            employee[i].netsalary = salary;
            cout << "Net Salary of " << employee[i].name << ": " << salary << endl;
            return employee[i].netsalary;
        }
        if (check == 0)
            cout << "Employee not found." << endl;
    } while (check == 0);
}

void addEmployee()
{
    if (employee_count >= 100)
    {
        cout << "You have reached maximum capacity." << endl;
        return;
    }

    int newId = 200 + employee_count + 1;
    employee[employee_count].id = newId;

    cin.ignore();
    cout << "Name:" << endl;
    getline(cin, employee[employee_count].name);
    cout << "Age:" << endl;
    cin >> employee[employee_count].age;
    cout << "Position:" << endl;
    cin.ignore();
    getline(cin, employee[employee_count].position);
    cout << "Phone number:" << endl;
    cin >> employee[employee_count].phone;
    cout << "Salary:";
    cin >> employee[employee_count].basicsalary;
    cout << "Password: ";
    cin.ignore();
    getline(cin, employee[employee_count].password);

    employee[employee_count].attendance[0] = { newId, 0, 0, 0 };
    employee_count++;
    cout << "Done! ID is: " << newId << endl;
}

void manageAttendance()
{
    int id;
    char choice;
    int check = 0;

    do
    {
        cout << "Enter Employee ID: ";
        cin >> id;
        for (int i = 0; i < employee_count; i++)
        {
            if (employee[i].id == id)
            {
                check = 1;
                int month;
                cout << "Enter Month (1-12): ";
                cin >> month;
                if (month < 1 || month > 12)
                {
                    cout << "Invalid Month!" << endl;
                    return;
                }
                cout << "Add Days Present for Month " << month << ": ";
                cin >> employee[i].attendance[month - 1].dayspresent;
                cout << "Add Days Absent for Month " << month << ": ";
                cin >> employee[i].attendance[month - 1].daysabsent;

                cout << "Attendance Recorded!" << endl;
                cout << employee[i].name << " has "
                    << employee[i].attendance[month - 1].dayspresent
                    << " days present and "
                    << employee[i].attendance[month - 1].daysabsent
                    << " days absent in month " << month << "." << endl;
                cout << "Do you want to manage attendance for another employee? (y/n): ";
                cin >> choice;
                break;
            }
        }

        if (check == 0)
        {
            cout << "Employee ID not found!" << endl;
        }
    } while (choice == 'y' || choice == 'Y' || check == 0);
}

void updateEmployee()
{
    if (employee_count == 0)
    {
        cout << "\nNo employees registered yet!" << endl;
        return;
    }
    int found = 0;
    do
    {
        cout << "\n--- Update Employee Data ---" << endl;
        cout << "Enter Employee ID to update: ";
        cin >> searchId;

        for (int i = 0; i < employee_count; i++)
        {
            if (employee[i].id == searchId)
            {
                found = 1;
                cout << "Employee Found! Updating: " << employee[i].name << endl;
                cout << "Enter New Age: ";
                cin >> employee[i].age;
                cout << "Enter New Phone: ";
                cin >> employee[i].phone;
                cout << "Enter New Position: ";
                cin.ignore();
                getline(cin, employee[i].position);
                cout << "Enter New Basic Salary: ";
                cin >> employee[i].basicsalary;

                cout << "\nData updated successfully!" << endl;
                break;
            }
        }

        if (found == 0)
        {
            cout << "Error: Employee with ID " << searchId << " not found."
                << endl;
        }
    } while (found == 0);
}

void deleteEmployee()
{
    if (employee_count == 0)
    {
        cout << "No employees to delete!\n";
        return;
    }
    int check = 0;
    do
    {
        cout << "Enter Employee ID to delete: ";
        cin >> deleteID;
        int index = -1;
        for (int i = 0; i < employee_count; i++)
        {
            if (employee[i].id == deleteID)
            {
                index = i;
                check = 1;
                break;
            }
        }

        if (index == -1)
        {
            cout << "Employee ID not found!\n";
            check = 0;
        }
        else
        {
            for (int i = index; i < employee_count - 1; i++)
                employee[i] = employee[i + 1];

            employee_count--;
            cout << "Employee deleted successfully!" << endl;
            cout << "Remaining employees: " << employee_count << endl;
        }
    } while (check == 0);
}

*/