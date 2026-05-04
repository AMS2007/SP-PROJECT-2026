#include "logic.h"

// your actual data lives here
Employee employee[100];
Admin admin[2];
int employeecount = 0;
int admincount = 0;

bool validateAdmin(const string& username, const string& password) {
    for (int i = 0; i < admincount; i++) {
        if (admin[i].username == username && admin[i].password == password)
            return true;
    }
    return false;
}

// validate employee id in edit employee panel
bool validateid(string id) {
    if (id.empty()) return false;
    try {
        int searchID = stoi(id);
        for (int i = 0; i < employeecount; i++) {
            if (employee[i].id == searchID)
                return true;
        }
    }
    catch (...) {
        return false;
    }
    return false;
}


bool validateEmployee(int id, const string& password, int& foundIndex) {
    for (int i = 0; i < employeecount; i++) {
        if (employee[i].id == id && employee[i].password == password) {
            foundIndex = i;
            return true;
        }
    }
    return false;
}

void addEmployee(const string& name, const string& password,
    long long salary, int age, long long phone) {
    if (employeecount >= 100) return;
    int newId = 200 + employeecount + 1;
    employee[employeecount].id = newId;
    employee[employeecount].name = name;
    employee[employeecount].password = password;
    employee[employeecount].basicsalary = salary;
    employee[employeecount].age = age;
    employee[employeecount].phone = phone;
    employee[employeecount].netsalary = 0;
    employee[employeecount].tax = 0;
    employee[employeecount].attendance = { newId, 0, 0 };
    employeecount++;
    saveAll();
}

void manageAttendance(int id, int daysPresent, int daysAbsent) {
    for (int i = 0; i < employeecount; i++) {
        if (employee[i].id == id) {
            employee[i].attendance.dayspresent += daysPresent;
            employee[i].attendance.daysabsent += daysAbsent;
            saveAll();
            return;
        }
    }
}

double calcSalary(int id, double overtimeHrs, double overtimeRate,
    double bonus, double salaryDeduction) {
    for (int i = 0; i < employeecount; i++) {
        if (employee[i].id == id) {
            double taxfree = employee[i].basicsalary * 0.86;
            double net = taxfree + (overtimeHrs * overtimeRate)
                + bonus
                - (employee[i].attendance.daysabsent * salaryDeduction);
            employee[i].tax = employee[i].basicsalary * 0.14;
            employee[i].netsalary = net;
            saveAll();
            return net;
        }
    }
    return -1;
}

void saveAll() {
    ofstream fE("employees.txt");
    for (int i = 0; i < employeecount; i++)
        fE << employee[i].id << " " << employee[i].name << " "
        << employee[i].basicsalary << " " << employee[i].tax << " "
        << employee[i].password << "\n";

    ofstream fA("attendance.txt");
    for (int i = 0; i < employeecount; i++)
        fA << employee[i].attendance.id << " "
        << employee[i].attendance.dayspresent << " "
        << employee[i].attendance.daysabsent << "\n";

    ofstream fAd("admins.txt");
    for (int i = 0; i < admincount; i++)
        fAd << admin[i].username << " " << admin[i].password << "\n";
}

void deleteEmployee(int deleteID, int& employee_count)
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
        }
    } while (check == 0);
}



/*float salarycalc()
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
}*/