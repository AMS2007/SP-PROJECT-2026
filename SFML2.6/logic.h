#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

using namespace std;
using namespace sf;


struct Attendance {
    int id;
    int dayspresent;
    int daysabsent;
};

struct Employee {
    int id, age;
    string name;
    string password;
    string position;
    long long phone, basicsalary;
    double netsalary, tax = 0.14, bonus, overtimehrs;
    Attendance attendance;
    Texture photo;
    Sprite profilePicture;
};

struct Admin {
    string username;
    string password;
    Texture photo;
    Sprite profilePicture;
};

extern Employee employee[100];
extern Admin admin[2];
extern int employeecount;
extern int admincount;

// these are all your functions, declared here so main.cpp can see them
bool validateid(string id);
bool validateAdmin(const string& username, const string& password);
bool validateEmployee(int id, const string& password, int& foundIndex);

void addEmployee(int& employee_count, const string& name, int age,
    const string& position, long long phone,
     const string& password, int basicsal);

void manageAttendance(int id, int daysPresent, int daysAbsent);


float calcSalary(int id, const string& basicSalInput, const string& bonusInput,
    const string& overtimeInput, int month, const string& deductionInput);
void saveAll();
void deleteEmployee(int deleteID, int& employeecount);
//float salarycalc();
//void  updateEmployee();
//void  deleteEmployee();