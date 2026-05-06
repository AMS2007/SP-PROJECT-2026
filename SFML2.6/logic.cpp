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

    int maxId = 200;
    for (int i = 0; i < employee_count; i++)
        if (employee[i].id > maxId)
            maxId = employee[i].id;

    int newId = maxId + 1;
    employee[employee_count].id = newId;
    employee[employee_count].name = name;
    employee[employee_count].age = age;
    employee[employee_count].position = position;
    employee[employee_count].phone = phone;
    employee[employee_count].password = password;
    employee[employee_count].attendance = { 0, 0 };
    employee[employee_count].basicsalary = basicsal;
    employee[employee_count].photo.loadFromFile("Images/user.png");
    employee[employee_count].profilePicture.setTexture(employee[employee_count].photo);
    employee_count++;
    employeecount = employee_count;
}
void updateEmployee(int& employee_count, int searchId, int age, long long phoneNumber, string position, int baseSalary)
{
    for (int i = 0; i < employee_count; i++)
    {
        if (employee[i].id == searchId)
        {
            employee[i].age = age;
            employee[i].phone = phoneNumber;
            employee[i].position = position;
            employee[i].basicsalary = baseSalary;
            return;
        }
    }
}

void manageAttendance(int id, int daysPresent, int daysAbsent, int month, int employee_count) {
    for (int i = 0; i < employee_count; i++) {
        if (employee[i].id == id) {
            employee[i].attendance.dayspresent[month-1] = daysPresent;
            employee[i].attendance.daysabsent[month-1] = daysAbsent;
            return;
        }
    }
}



void deleteEmployee(int deleteID, int& employee_count)
{
    // find the employee
    int index = -1;
    for (int i = 0; i < employee_count; i++)
    {
        if (employee[i].id == deleteID)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        cout << "Employee ID not found!" << endl;
        return;
    }

    // shift all employees after index one position left
    for (int i = index; i < employee_count - 1; i++)
        employee[i] = employee[i + 1];

    employee_count--;
    employeecount = employee_count;
    
}

float calcSalary(int id, const string& bonusInput,
    const string& overtimeInput, int month, const string& deductionInput, int&employee_count)
{
    for (int i = 0; i < employee_count; i++)
    {
        if (employee[i].id == id)
        {
            
            float bonus = stof(bonusInput);
            float overtime = stof(overtimeInput);
            float overtimerate = 50.f;  // set your overtime rate here
            float salarydeduction = stof(deductionInput); // deduction per absent day

            int daysAbsent = employee[i].attendance.daysabsent[month-1];

            float taxfree = employee[i].basicsalary - (employee[i].basicsalary * 0.14f);
            float salary = taxfree + (overtime * overtimerate)
                + bonus - (daysAbsent * salarydeduction);

            employee[i].tax = employee[i].basicsalary * 0.14f;
            employee[i].netsalary = salary;
            employee[i].bonus = bonus;
            employee[i].overtimehrs = overtime;
            return salary;
        }
    }
    return -1; // employee not found
}


