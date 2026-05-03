/*#include <string>
#pragma once
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


float salarycalc();
void  addEmployee();
void  manageAttendance();
void  updateEmployee();
void  deleteEmployee();
*/