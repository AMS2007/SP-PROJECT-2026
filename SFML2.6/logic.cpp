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
bool validateid(string id , int & employee_count) {

    if (id.empty()) return false;
    try {
        int searchID = stoi(id);
        for (int i = 0; i < employee_count; i++) {
            if (employee[i].id == searchID)
                return true;
        }
    }
    catch (...) {
        return false;
    }
    return false;
}


bool validateEmployee(int id, const string& password, int& foundIndex,int & employee_count) {
    for (int i = 0; i < employee_count; i++) {
        if (employee[i].id == id && employee[i].password == password) {
            foundIndex = i;
            return true;
        }
    }
    return false;
}

void addEmployee(int& employee_count, const string& name, int age,
    const string& position, long long phone,
     const string& password, int basicsal)
{
    if (employee_count >= 100) return;

    int newId = 200 + employee_count + 1;
    employee[employee_count].id = newId;
    employee[employee_count].name = name;
    employee[employee_count].age = age;
    employee[employee_count].position = position;
    employee[employee_count].phone = phone;
    employee[employee_count].password = password;
    employee[employee_count].attendance = { 0, 0 };
    employee[employee_count].basicsalary = basicsal;
    employee_count++;
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

float calcSalary(int id, const string& basicSalInput, const string& bonusInput,
    const string& overtimeInput, int month, const string& deductionInput)
{
    for (int i = 0; i < employeecount; i++)
    {
        if (employee[i].id == id)
        {
            float basicsalary = stof(basicSalInput);
            float bonus = stof(bonusInput);
            float overtime = stof(overtimeInput);
            float overtimerate = 50.f;  // set your overtime rate here
            float salarydeduction = stof(deductionInput); // deduction per absent day

            int daysAbsent = employee[i].attendance.daysabsent;

            float taxfree = basicsalary - (basicsalary * 0.14f);
            float salary = taxfree + (overtime * overtimerate)
                + bonus - (daysAbsent * salarydeduction);

            employee[i].tax = basicsalary * 0.14f;
            employee[i].netsalary = salary;
            employee[i].bonus = bonus;
            employee[i].overtimehrs = overtime;
            employee[i].basicsalary = basicsalary;

            //ssaveAll();
            return salary;
        }
    }
    return -1; // employee not found
}


