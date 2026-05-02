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

bool validateid(string id) {
    for (int i = 0; i < employeecount; i++) {
        if (employee[i].id == stoi(id)) {
            return true;
        }
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