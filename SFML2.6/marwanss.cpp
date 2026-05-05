#include <iostream>
#include <string>
using namespace std;
const int months=12;
struct Attendance 
{
	long long dayspresent, daysabsent;
};
struct Employee
{
	long long id, age, phone, basicsalary;
	string name, position, password;
	float bonus, tax, netsalary, overtimehrs;
	Attendance attendance[months];
} employee[100];

void Employeepanel(int index)
{
int reqmonth=0;
	while (true) {
        cout << "1.View personal information \n2.view salary details \n3.View attendance record \n4.Logout \n choice: ";
	int choice;
	cin >> choice;
	while (choice > 4 || choice < 1) 
	{
		cout << "Invalid Choice Please Retry" << endl;
		cin >> choice;
	}
	if (choice == 1){
		cout << "Name is: " << employee[index].name <<endl;
        cout << "Age is: " << employee[index].age <<endl;
        cout << "Phone Number is: " <<employee[index].phone<<endl;
	}
	else if (choice == 2)
	{
		cout << "salary: " << employee[index].netsalary << endl;
	}
	else if (choice == 3)
	{
		while(true)
		{
		cout<<"Enter the month to be viewed (1-12):\n ";
		cin>>reqmonth;
		if(reqmonth<1||reqmonth>12)
		{ 
		cout<<"invalid months!Try again \n";
		continue;
		}

		else{
	
		cout << "Days Attendant in month " << reqmonth << ": " << employee[index].attendance[reqmonth-1].dayspresent << endl;
		cout << "Days Absent in month "<<  reqmonth << ": " << employee[index].attendance[reqmonth-1].daysabsent << endl;
	break;	
	}
		}
	}

    else if ( choice == 4) {      // returns to employee login function
          return;              
    }
    }
}

int employee_count = 5; // it equals 5 as there are 5 pre stored employees
void Employeelogin()
{
	while (true) {
        int ID;
	string pass;
	cout << "Enter ID: ";
	cin >> ID;
	cout << "Enter Password: ";
	cin.ignore();
	getline(cin,pass);
	for (int i = 0; i < employee_count; i++)
	{
		if ((ID == employee[i].id) && (pass == employee[i].password))
		{
			cout << "Welcome\n";
			Employeepanel(i); // the index is sent to the panel function to be used in displaying information
			return;  //returns to mainmenu when logout is selected
		}
		else
		{
			continue; // if the id & pass don't match the employee in iteration it will go to the next one
		}
	}
    cout << "Try again \n"; // displayed when the loop finishes and doesn't find the match
    }
}
int MainMenu()
{
	int choice;
	cout << "Welcome!\n Choose:\n1.Admin login\n2.Employee login\n3.Exit\n\nChoice: ";
	cin >> choice;
	if (choice == 1)
		cout << "Admin" << endl;
	else if (choice == 2) {
		Employeelogin();

    }
	else if (choice == 3)
	{
		cout << "Exit completed\n______________\nhave a nice day" << endl;
		
	}
	else
	{
		cout << "Error. Try again." << endl; // validation
		MainMenu();
	}
	return choice;
}

int main()
{
	employee[0] = {
		2025001,
		19,
		201142275561,
		5000,
		"Ahmed",
		"HR",
		"AHMED123",
		4000,
		0.14,
		100000,
		5,{{24,7},{25,5},{30,0},{28,2},{22,7},{30,0},{24,7},{25,5},{30,0},{28,2},{22,7},{30,0}}

	};
	employee[1] = {
		2025002,
		22,
		201562275961,
		5000,
		"Ebram",
		"IT",
		"EBRAM1234",
		4000,
		0.14,
		100000,
		5,
		{{24,7},{25,5},{30,0},{28,2},{22,7},{30,0},{24,7},{25,5},{30,0},{28,2},{22,7},{30,0}}
	};
	employee[2] = {
		2025003,
		18,
		201047275061,
		5000,
		"Sofia",
		"marketing",
		"SOFIA235",
		4000,
		0.14,
		100000,
		5,
		{{24,7},{25,5},{30,0},{28,2},{22,7},{30,0},{24,7},{25,5},{30,0},{28,2},{22,7},{30,0}}
	};
	employee[3] = {
		2025004,
		24,
		201142243669,
		5000,
		"Elona",
		"IT",
		"ELONA642",
		4000,
		0.14,
		100000,
		5,
		{{24,7},{25,5},{30,0},{28,2},{22,7},{30,0},{24,7},{25,5},{30,0},{28,2},{22,7},{30,0}}
	};
	employee[4] = {
		2025005,
		19,
		20114227556,
		5000,
		"Marwan",
		"Devolper",
		"MARWAN1237",
		4000,
		0.14,
		100000,
		5,
	{{24,7},{25,5},{30,0},{28,2},{22,7},{30,0},{24,7},{25,5},{30,0},{28,2},{22,7},{30,0}}};
int choice;
	do {
		choice = MainMenu();
	} while (choice != 3);
	return 0;
}