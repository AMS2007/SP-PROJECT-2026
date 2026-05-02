
#include <string>
# pragma once
using namespace std;
struct Employee
{
	long long id, age, phone, basicsalary;
	string name, position, password;
	float bonus, tax, netsalary, overtimehrs;
};
extern Employee employee[100];
extern int employee_count;

void Employeepanel(int index);
void Employeelogin();
int MainMenu();