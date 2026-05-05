#include "logic.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <vector>
#include <sstream>
#include <SFML/Button.hpp>
#include <SFML/sfmlbutton.hpp>
#include <SFML/EllipseButton.hpp>
#include <SFML/RectButton.hpp>
using namespace sf;
using namespace std;


// panels
enum GameState {
    Menu,
    adminLogin,
    employeeLogin,
    adminPanel,
    employeePanel,
    addEmployeePanel,
    editEmployeePanel,
    updatePanel,
    attendanceEditPanel,
    salaryCalcPanel,
    deletePanel,
    viewPanel,
    salaryPanel,
    attendancePanel,
    attendanceOkPanel,
    salaryViewPanel,
    netSalaryPanel,
    attendanceViewPanel,
    addedsuccessfully,
    zerotrailsleft,
};


struct Textboxdata {
    RectangleShape box;
    Text label;
    Text displayText;
    string input;
    bool isFocused = false;

    Color defaultOutline = Color::Black;
    Color focusedOutline = Color(1, 46, 90);

    // Constructor to set everything up at once
    Textboxdata(Font& font, Vector2f size, Vector2f position,
        const string& labelStr, unsigned int fontSize = 20)
    {
        // Box
        box.setSize(size);
        box.setPosition(position);
        box.setFillColor(Color::White);
        box.setOutlineColor(defaultOutline);
        box.setOutlineThickness(2);

        // Label above the box
        label.setFont(font);
        label.setFillColor(Color::Black);
        label.setCharacterSize(fontSize + 4);
        label.setString(labelStr);
        label.setPosition(position.x, position.y - 30);

        // Text inside the box
        displayText.setFont(font);
        displayText.setFillColor(Color::Black);
        displayText.setCharacterSize(fontSize);
        displayText.setString("");
        displayText.setPosition(position.x + 5, position.y + 5);
    }

    void handleEvent(const Event& event, const RenderWindow& window) {
        // Focus on click
        if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
            Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
            isFocused = box.getGlobalBounds().contains(mousePos);
            box.setOutlineColor(isFocused ? focusedOutline : defaultOutline);
            box.setOutlineThickness(isFocused ? 3 : 2);
        }

        // Typing
        if (event.type == Event::TextEntered && isFocused) {
            if (event.text.unicode == '\b') {
                if (!input.empty())
                    input.pop_back();
            }
            else if (event.text.unicode < 128) {
                string test = input + static_cast<char>(event.text.unicode);
                displayText.setString(test);
                if (displayText.getGlobalBounds().width < box.getSize().x - 10)
                    input = test;
                else
                    displayText.setString(input);
            }
            displayText.setString(input);
        }
    }
    void draw(RenderWindow& window) {
        window.draw(box);
        window.draw(label);
        window.draw(displayText);
    }

    void clear() {
        input.clear();
        displayText.setString("");
    }
};

// start menu buttons struct
struct ButtonData {
    RectButton* mButton = nullptr;
    string label;
    Color defaultColor;
    Color hoverColor;
    Texture texture;

    bool loadImage(const string& path) {
        if (!texture.loadFromFile(path)) {
            cerr << "ERROR :: Could not load: " << path << endl;
            return false;
        }
        mButton->button.setTexture(&texture);
        mButton->setButtonLabel(24, "");  // clear label inside here
        return true;
    }
}adminButton,
employeeButton,
exitButton,
backButton,
loginButton,
addButton,
logoutButton,
updateButton,
enterButton,
attendanceButton,
salaryButton,
deleteButton,
viewButton,
viewSalaryButton,
viewAttendButton,
deleteButtonOkay,
enterOkButton,
exitAppButton;

struct TextData {
    sf::Text text;

    TextData() {}

    TextData(sf::Font& font, const string& str, unsigned int charSize, sf::Color color, sf::Vector2f position) {
        text.setFont(font);
        text.setString(str);
        text.setCharacterSize(charSize);
        text.setFillColor(color);
        text.setPosition(position);

    }

    void centerOrigin() {
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    }
};

void rebuildData(TextData*& empID, TextData*&empName, TextData*& empPhone, TextData*& empPosition, TextData*& empAge, int employee_count, Font& font, float width, float height) {
    delete[] empID;
    delete[] empName;
    delete[] empPhone;
    delete[] empPosition;
    delete[] empAge;

    empID = new TextData[employee_count];
    empName = new TextData[employee_count];
    empPhone = new TextData[employee_count];
    empPosition = new TextData[employee_count];
    empAge = new TextData[employee_count];

    for (int i = 0; i < employee_count; i++) {
        empID[i] = TextData(font, to_string(employee[i].id), 20, Color::Black, Vector2f(width / 4.f * 3.f - 5.f, height / 4.f + 310.f));
        empName[i] = TextData(font, employee[i].name, 64, Color(1, 46, 90), Vector2f(width / 4.f - 250.f, height / 4.f - 100));
        empPhone[i] = TextData(font, to_string(employee[i].phone), 20, Color::Black, Vector2f(width / 4.f * 3.f + 125, height / 4.f + 250.f));
        empPosition[i] = TextData(font, employee[i].position, 26, Color::Black, Vector2f(width / 4.f - 247.f, height / 4.f - 14.f));
        empAge[i] = TextData(font, to_string(employee[i].age), 20, Color::Black, Vector2f(width / 4.f * 3.f + 5, height / 4.f + 370.f));
        employee[i].profilePicture.setTexture(employee[i].photo);
        employee[i].profilePicture.setOrigin(
            employee[i].profilePicture.getLocalBounds().width / 2.f,
            employee[i].profilePicture.getLocalBounds().height / 2.f
        );
        employee[i].profilePicture.setPosition(Vector2f(width / 4.f * 3.f + 45, height / 4.f + 40.f));
        employee[i].profilePicture.setScale(0.37f, 0.37f);
    }

}


int main()
{
    unsigned int height = 800; // height of window
    unsigned int width = 1600; // width of window

    employee[0] = { 201, 19, "Ahmed",  "Ahmed123",    "IT Manager",             201142275561, 90000};
    employee[1] = { 202, 19, "Ebram",  "Ebram123",    "IT",                     201152263354, 40000};
    employee[2] = { 203, 18, "Mona",   "Mona123",     "Marketing Manaager",     201006034700, 10000};
    employee[3] = { 204, 20, "Steven", "Steven123",   "Marketing",              201006034720, 30000};
    employee[4] = { 205, 18, "Marwan", "Marwan123",   "Developer Manager",      201102446612, 60000};

    employee[0].photo.loadFromFile("Images/ahmed.png");
    employee[1].photo.loadFromFile("Images/ebram.png");
    employee[2].photo.loadFromFile("Images/mona.png");
    employee[3].photo.loadFromFile("Images/steven.png");
    employee[4].photo.loadFromFile("Images/marwan.png");
    for (int i = 0; i < 5; i++)
    {
        employee[i].profilePicture.setTexture(employee[i].photo);
        employee[i].profilePicture.setOrigin(
            employee[i].profilePicture.getLocalBounds().width / 2.f,
            employee[i].profilePicture.getLocalBounds().height / 2.f
        );
        employee[i].profilePicture.setPosition(Vector2f(width / 4.f * 3.f + 45, height / 4.f + 40.f));
        employee[i].profilePicture.setScale(0.37f, 0.37f);
    }
    int employee_count = 5;
    float calculatedNetSalary = 0.f;
    GameState currentState = Menu; // screen when you open window

    RenderWindow window(VideoMode(width, height), "Employee Payroll Management System By 2202 GROUP");

    window.setFramerateLimit(60); // frame limit of window
    window.setKeyRepeatEnabled(true); // one press each time

    Font font; // blockletter font
    if (!font.loadFromFile("Fonts/Ranade-Regular.otf")) {
        cerr << "ERROR :: COULD_NOT_OPEN_FROM_FILE :: MAIN::Fonts/Ranade-Regular.otf" << endl;
    }

    // top of the window details
    RectangleShape topBar(Vector2f(1600, 60));
    topBar.setPosition(0, 0);
    topBar.setFillColor(Color(1, 46, 90));
    // top of the window details end

    // admin button details
    adminButton.label = "I am an Admin";
    adminButton.defaultColor = Color(1, 46, 90);
    adminButton.hoverColor = Color(101, 192, 155);

    Text admin_text;
    admin_text.setFont(font);
    admin_text.setCharacterSize(24);
    admin_text.setString(adminButton.label);
    FloatRect admin_tb = admin_text.getGlobalBounds();
    Vector2f admin_size(admin_tb.width * 1.5f, admin_tb.height * 2.f);

    adminButton.mButton = new RectButton(font, admin_size, Vector2f(width / 4.f - admin_size.x / 2.f, height / 4.f * 3.f - admin_size.y / 2.f));
    adminButton.mButton->setButtonLabel(24, adminButton.label);
    adminButton.mButton->setLabelColor(Color::White);
    adminButton.mButton->setButtonColor(adminButton.defaultColor);
    // end admin button details

    // employee button details
    employeeButton.label = "I am an Employee";
    employeeButton.defaultColor = Color(1, 46, 90);
    employeeButton.hoverColor = Color(101, 192, 155);

    Text employee_text;
    employee_text.setFont(font);
    employee_text.setCharacterSize(24);
    employee_text.setString(employeeButton.label);
    FloatRect employee_tb = employee_text.getGlobalBounds();
    Vector2f employee_size(employee_tb.width * 1.5f, employee_tb.height * 2.f);

    employeeButton.mButton = new RectButton(font, employee_size, Vector2f(width / 4.f * 3.f - employee_size.x / 2.f, height / 4.f * 3.f - employee_size.y / 2.f));
    employeeButton.mButton->setButtonLabel(24, employeeButton.label);
    employeeButton.mButton->setLabelColor(Color::White);
    employeeButton.mButton->setButtonColor(employeeButton.defaultColor);
    // end employee button details

    bool Showerror = false;

    Text emptyloginbox;
    emptyloginbox.setFont(font);
    emptyloginbox.setFillColor(Color::Red);
    emptyloginbox.setCharacterSize(25);
    emptyloginbox.setPosition(1050, 500);
    emptyloginbox.setString("Fields are empty");


    // exit app button if trails reached 3 
    exitAppButton.mButton = new RectButton(font, Vector2f(60.f, 50.f), Vector2f(5.f, 5.f));
    exitAppButton.mButton->setButtonLabel(24, exitAppButton.label);
    exitAppButton.mButton->setButtonLabel(24, "EXIT");


    // exit button details
    exitButton.mButton = new RectButton(font, Vector2f(60.f, 50.f), Vector2f(5.f, 5.f));
    exitButton.mButton->setButtonLabel(24, exitButton.label);
    exitButton.loadImage("Images/exit.png");
    exitButton.mButton->setButtonLabel(24, "");
    // end exit button details

    // back button details
    backButton.mButton = new RectButton(font, Vector2f(60.f, 50.f), Vector2f(5.f, 5.f));
    backButton.mButton->setButtonLabel(24, backButton.label);
    backButton.loadImage("Images/left-arrow.png");
    backButton.mButton->setButtonLabel(24, "");
    // back exit button details

    // log in button details
    loginButton.label = "Log In";
    loginButton.defaultColor = Color(1, 46, 90);
    loginButton.hoverColor = Color(101, 192, 155);

    Text login_text;
    login_text.setFont(font);
    login_text.setCharacterSize(36);
    login_text.setString(loginButton.label);
    FloatRect login_tb = login_text.getGlobalBounds();
    Vector2f login_size(login_tb.width * 1.5f, login_tb.height * 2.f);

    loginButton.mButton = new RectButton(font, login_size, Vector2f(width / 2.f - login_size.x / 2.f, height / 2.f - login_size.y / 2.f + 160.f));

    loginButton.mButton->setButtonLabel(36, loginButton.label);
    loginButton.mButton->setLabelColor(Color::White);
    loginButton.mButton->setButtonColor(loginButton.defaultColor);
    // end log in button details

    // add employee button details
    addButton.label = "Add Employee";
    addButton.defaultColor = Color(1, 46, 90);
    addButton.hoverColor = Color(101, 192, 155);

    Text add_text;
    add_text.setFont(font);
    add_text.setCharacterSize(36);
    add_text.setString(addButton.label);
    FloatRect add_tb = add_text.getGlobalBounds();
    Vector2f add_size(add_tb.width * 1.5f, add_tb.height * 2.f);

    addButton.mButton = new RectButton(font, add_size, Vector2f(width / 4.f * 3.f - add_size.x / 2.f, height / 4.f * 3.f - add_size.y / 2.f - 100.f));
    addButton.mButton->setButtonLabel(36, addButton.label);
    addButton.mButton->setLabelColor(Color::White);
    addButton.mButton->setButtonColor(addButton.defaultColor);
    // end add employee button details

    // log out button details
    logoutButton.label = "Log Out";
    logoutButton.defaultColor = Color(1, 46, 90);
    logoutButton.hoverColor = Color(31, 11, 64);

    Text logout_text;
    logout_text.setFont(font);
    logout_text.setCharacterSize(24);
    logout_text.setString(logoutButton.label);
    FloatRect logout_tb = logout_text.getGlobalBounds();
    Vector2f logout_size(logout_tb.width * 1.5f, logout_tb.height * 1.f);

    logoutButton.mButton = new RectButton(font, logout_size, Vector2f(0.f, 20.f));

    logoutButton.mButton->setButtonLabel(24, logoutButton.label);
    logoutButton.mButton->setLabelColor(Color::White);
    logoutButton.mButton->setButtonColor(logoutButton.defaultColor);
    // end log in button details

    // update employee button details
    updateButton.label = "Update Employee";
    updateButton.defaultColor = Color(1, 46, 90);
    updateButton.hoverColor = Color(101, 192, 155);

    Text update_text;
    update_text.setFont(font);
    update_text.setCharacterSize(36);
    update_text.setString(updateButton.label);
    FloatRect update_tb = update_text.getGlobalBounds();
    Vector2f update_size(update_tb.width * 1.2f, update_tb.height * 1.7f);

    updateButton.mButton = new RectButton(font, update_size, Vector2f(width / 4.f - update_size.x / 2.f - 20.f, height / 4.f - update_size.y / 2.f + 100.f));

    updateButton.mButton->setButtonLabel(36, updateButton.label);
    updateButton.mButton->setLabelColor(Color::White);
    updateButton.mButton->setButtonColor(updateButton.defaultColor);
    // end update employee button details

    // enter employee button details
    enterButton.label = "Enter";
    enterButton.defaultColor = Color(1, 46, 90);
    enterButton.hoverColor = Color(101, 192, 155);

    Text enter_text;
    enter_text.setFont(font);
    enter_text.setCharacterSize(24);
    enter_text.setString(enterButton.label);
    FloatRect enter_tb = enter_text.getGlobalBounds();
    Vector2f enter_size(enter_tb.width * 1.5f, enter_tb.height * 2.f);

    enterButton.mButton = new RectButton(font, enter_size, Vector2f(width / 4.f - enter_size.x / 2.f, height / 4.f * 3.f - enter_size.y / 2.f));

    enterButton.mButton->setButtonLabel(24, enterButton.label);
    enterButton.mButton->setLabelColor(Color::White);
    enterButton.mButton->setButtonColor(enterButton.defaultColor);
    // end enter employee button details

    // attendance button details
    attendanceButton.label = "Record Attendance";
    attendanceButton.defaultColor = Color(1, 46, 90);
    attendanceButton.hoverColor = Color(101, 192, 155);

    Text attendance_text;
    attendance_text.setFont(font);
    attendance_text.setCharacterSize(36);
    attendance_text.setString(attendanceButton.label);
    FloatRect attendance_tb = attendance_text.getGlobalBounds();
    Vector2f attendance_size(attendance_tb.width * 1.2f, attendance_tb.height * 2.5f);

    attendanceButton.mButton = new RectButton(font, attendance_size, Vector2f(width / 4.f - attendance_size.x / 2.f - 5.f, height / 2.f - attendance_size.y / 2.f + 35.f));

    attendanceButton.mButton->setButtonLabel(36, attendanceButton.label);
    attendanceButton.mButton->setLabelColor(Color::White);
    attendanceButton.mButton->setButtonColor(attendanceButton.defaultColor);
    // end attendance button details

    // salary button details
    salaryButton.label = "Calculate Salary";
    salaryButton.defaultColor = Color(1, 46, 90);
    salaryButton.hoverColor = Color(101, 192, 155);

    Text salary_text;
    salary_text.setFont(font);
    salary_text.setCharacterSize(36);
    salary_text.setString(salaryButton.label);
    FloatRect salary_tb = salary_text.getGlobalBounds();
    Vector2f salary_size(salary_tb.width * 1.2f, salary_tb.height * 1.7f);

    salaryButton.mButton = new RectButton(font, salary_size, Vector2f(width / 4.f - salary_size.x / 2.f - 37.f, height / 4.f * 3.f - salary_size.y / 2.f - 35.f));

    salaryButton.mButton->setButtonLabel(36, salaryButton.label);
    salaryButton.mButton->setLabelColor(Color::White);
    salaryButton.mButton->setButtonColor(salaryButton.defaultColor);
    // end salary button details

    // delete button details
    deleteButton.label = "Delete Employee";
    deleteButton.defaultColor = Color(1, 46, 90);
    deleteButton.hoverColor = Color(1, 46, 90);

    Text delete_text;
    delete_text.setFont(font);
    delete_text.setCharacterSize(36);
    delete_text.setString(deleteButton.label);
    FloatRect delete_tb = delete_text.getGlobalBounds();
    Vector2f delete_size(delete_tb.width * 1.2f, delete_tb.height * 1.7f);

    deleteButton.mButton = new RectButton(font, delete_size, Vector2f(width / 4.f - delete_size.x / 2.f - 40.f, height / 4.f * 3.f - delete_size.y / 2.f + 90.f));

    deleteButton.mButton->setButtonLabel(36, deleteButton.label);
    deleteButton.mButton->setLabelColor(Color::White);
    deleteButton.mButton->setButtonColor(deleteButton.defaultColor);
    // end delete button details

    // view button details
    viewButton.label = "View Personal Details";
    viewButton.defaultColor = Color(1, 46, 90);
    viewButton.hoverColor = Color(101, 192, 155);

    Text view_text;
    view_text.setFont(font);
    view_text.setCharacterSize(36);
    view_text.setString(viewButton.label);
    FloatRect view_tb = view_text.getGlobalBounds();
    Vector2f view_size(view_tb.width * 1.2f, view_tb.height * 2.f);

    viewButton.mButton = new RectButton(font, view_size, Vector2f(width / 2.f - view_size.x / 2.f, height / 4.f - view_size.y / 2.f + 100.f));

    viewButton.mButton->setButtonLabel(36, viewButton.label);
    viewButton.mButton->setLabelColor(Color::White);
    viewButton.mButton->setButtonColor(viewButton.defaultColor);
    // end view button details

    // salary button details
    viewSalaryButton.label = "View Salary";
    viewSalaryButton.defaultColor = Color(1, 46, 90);
    viewSalaryButton.hoverColor = Color(101, 192, 155);

    Text vSalary_text;
    vSalary_text.setFont(font);
    vSalary_text.setCharacterSize(36);
    vSalary_text.setString(viewSalaryButton.label);
    FloatRect vSalary_tb = vSalary_text.getGlobalBounds();
    Vector2f vSalary_size(vSalary_tb.width * 1.2f, vSalary_tb.height * 1.7f);

    viewSalaryButton.mButton = new RectButton(font, vSalary_size, Vector2f(width / 2.f - vSalary_size.x / 2.f, height / 4.f - vSalary_size.y / 2.f + 200.f));

    viewSalaryButton.mButton->setButtonLabel(36, viewSalaryButton.label);
    viewSalaryButton.mButton->setLabelColor(Color::White);
    viewSalaryButton.mButton->setButtonColor(viewSalaryButton.defaultColor);
    // end salary button details

    // attendance button details
    viewAttendButton.label = "View Attendance Records";
    viewAttendButton.defaultColor = Color(1, 46, 90);
    viewAttendButton.hoverColor = Color(101, 192, 155);

    Text vAttend_text;
    vAttend_text.setFont(font);
    vAttend_text.setCharacterSize(36);
    vAttend_text.setString(viewAttendButton.label);
    FloatRect vAttend_tb = vAttend_text.getGlobalBounds();
    Vector2f vAttend_size(vAttend_tb.width * 1.2f, vAttend_tb.height * 2.f);

    viewAttendButton.mButton = new RectButton(font, vAttend_size, Vector2f(width / 2.f - vAttend_size.x / 2.f, height / 4.f - vAttend_size.y / 2.f + 300.f));

    viewAttendButton.mButton->setButtonLabel(36, viewAttendButton.label);
    viewAttendButton.mButton->setLabelColor(Color::White);
    viewAttendButton.mButton->setButtonColor(viewAttendButton.defaultColor);
    // end attendance button details


    // okay button deleted succefully details
    deleteButtonOkay.label = "Okay";
    deleteButtonOkay.defaultColor = Color(1, 46, 90);
    deleteButtonOkay.hoverColor = Color(101, 192, 155);

    Text deleteButtonTextOkay;
    deleteButtonTextOkay.setFont(font);
    deleteButtonTextOkay.setCharacterSize(36);
    deleteButtonTextOkay.setString(deleteButtonOkay.label);
    FloatRect deleteButtonText1 = deleteButtonTextOkay.getGlobalBounds();
    Vector2f Okay_size(deleteButtonText1.width * 1.2f, deleteButtonText1.height * 2.f);

    deleteButtonOkay.mButton = new RectButton(font, Okay_size, Vector2f(width / 2.f - Okay_size.x / 2.f, height / 4.f * 3.f - Okay_size.y / 2.f + 100.f));

    deleteButtonOkay.mButton->setButtonLabel(36, deleteButtonOkay.label);
    deleteButtonOkay.mButton->setLabelColor(Color::White);
    deleteButtonOkay.mButton->setButtonColor(deleteButtonOkay.defaultColor);
    // okay button deleted succefully details END

    // enter attendance button details
    enterOkButton.label = "Enter";
    enterOkButton.defaultColor = Color(1, 46, 90);
    enterOkButton.hoverColor = Color(101, 192, 155);

    Text enterA_text;
    enterA_text.setFont(font);
    enterA_text.setCharacterSize(24);
    enterA_text.setString(enterOkButton.label);
    FloatRect enterA_tb = enterA_text.getGlobalBounds();
    Vector2f enterA_size(enterA_tb.width * 1.5f, enterA_tb.height * 2.f);

    enterOkButton.mButton = new RectButton(font, enterA_size, Vector2f(width / 2.f - enterA_size.x / 2.f, height / 4.f * 3.f - enterA_size.y / 2.f + 100.f));

    enterOkButton.mButton->setButtonLabel(24, enterOkButton.label);
    enterOkButton.mButton->setLabelColor(Color::White);
    enterOkButton.mButton->setButtonColor(enterOkButton.defaultColor);
    // end enter employee button details


    // menu admin and employee icons
    Texture adminTexture;
    adminTexture.loadFromFile("Images/employee_622846.png");
    Sprite adminSprite;
    adminSprite.setTexture(adminTexture);
    adminSprite.setOrigin(adminSprite.getLocalBounds().width / 2, adminSprite.getLocalBounds().height / 2);
    adminSprite.setPosition(width / 4.f, height / 2.f - 40.f); // (400, 360)
    adminSprite.setScale(0.75f, 0.75f);

    Texture employeeTexture;
    employeeTexture.loadFromFile("Images/employee_622850.png");
    Sprite employeeSprite;
    employeeSprite.setTexture(employeeTexture);
    employeeSprite.setOrigin(employeeSprite.getLocalBounds().width / 2.f, employeeSprite.getLocalBounds().height / 2.f);
    employeeSprite.setPosition(width / 4.f * 3.f, height / 2.f - 40.f); // (1200,360)
    employeeSprite.setScale(0.75f, 0.75f);
    // end admin and employee icons

    // admin login image 
    Texture adminImage;
    adminImage.loadFromFile("Images/01784fb2-e758-4ed7-ad0b-1bd195549471.jpg");
    Sprite adminImageSprite;
    adminImageSprite.setTexture(adminImage);
    adminImageSprite.setOrigin(adminImageSprite.getLocalBounds().width / 2, adminImageSprite.getLocalBounds().height / 2);
    adminImageSprite.setPosition(width / 2.f, height / 2.f); // (450,435)
    adminImageSprite.setScale(0.3f, 0.3f);
    // admin login image end

    // textbox
    Textboxdata idBox(font, Vector2f(300, 40), Vector2f(650, 300), "Enter Your Username:");
    Textboxdata passwordBox(font, Vector2f(300, 40), Vector2f(650, 400), "Enter Your Password:");
    Textboxdata idBoxEmp(font, Vector2f(300, 40), Vector2f(650, 300), "Enter Your ID Number:");
    Textboxdata passwordBoxEmp(font, Vector2f(300, 40), Vector2f(650, 400), "Enter Your Password:");
    Textboxdata employeeidadminpanel(font, Vector2f(300, 40), Vector2f(215, 350), "Enter The Employee ID:");
    Textboxdata monthBox(font, Vector2f(300, 40), Vector2f(width / 4.f, height / 4.f), "Enter Month (1-12):");
    Textboxdata presentBox(font, Vector2f(300, 40), Vector2f(width / 4.f, height / 4.f + 100.f), "Enter Days Present:");
    Textboxdata absentBox(font, Vector2f(300, 40), Vector2f(width / 4.f, height / 4.f + 200.f), "Enter Days Absent:");
    Textboxdata basicSalBox(font, Vector2f(300, 40), Vector2f(width / 4.f, height / 4.f + 100.f), "Enter Basic Salary:");
    Textboxdata bonusBox(font, Vector2f(300, 40), Vector2f(width / 4.f, height / 4.f + 200.f), "Enter Bonus:");
    Textboxdata overtimeBox(font, Vector2f(300, 40), Vector2f(width / 4.f, height / 4.f + 300.f), "Enter Overtime Hours:");
    Textboxdata deductionBox(font, Vector2f(300, 40), Vector2f(width / 4.f, height / 4.f + 400.f), "Enter Salary Deduction (per day absent):");
    Textboxdata NameBox(font, Vector2f(300, 40), Vector2f(width / 4.f, height / 4.f), "Enter Name: ");
    Textboxdata PassBox(font, Vector2f(300, 40), Vector2f(width / 4.f, height / 4.f + 100.f), "Enter Password: ");
    Textboxdata AgeBox2(font, Vector2f(300, 40), Vector2f(width / 4.f, height / 4.f +200.f), "Enter Age: ");
    Textboxdata PositionBox(font, Vector2f(300, 40), Vector2f(width / 4.f, height / 4.f+300.f), "Enter Position: ");
    Textboxdata PhoneNumberBox(font, Vector2f(300, 40), Vector2f(width / 4.f, height / 4.f+400.f), "Enter Phone Number: ");
    Textboxdata BasicSalBox(font, Vector2f(300, 40), Vector2f(width / 4.f, height / 4.f + 500.f), "Enter Starting Salary: ");


    // textbox end

    // employee login image 
    Texture employeeImage;
    employeeImage.loadFromFile("Images/01784fb2-e758-4ed7-ad0b-1bd195549471.jpg");
    Sprite employeeImageSprite;
    employeeImageSprite.setTexture(employeeImage);
    employeeImageSprite.setOrigin(employeeImageSprite.getLocalBounds().width / 2, employeeImageSprite.getLocalBounds().height / 2);
    employeeImageSprite.setPosition(width / 2.f, height / 2.f); // (450,435)
    employeeImageSprite.setScale(0.3f, 0.3f);
    // employee login image end

    // welcome text
    Text welc;
    welc.setCharacterSize(64);
    welc.setFont(font);
    welc.setFillColor(Color(1, 46, 90));
    welc.setString("Welcome!");
    welc.setOrigin(welc.getLocalBounds().width / 2.f, welc.getLocalBounds().height / 2.f);
    welc.setPosition(width / 2.f, height / 4.f - 50.f);

    Text welc_admin;
    welc_admin.setCharacterSize(64);
    welc_admin.setFont(font);
    welc_admin.setFillColor(Color(1, 46, 90));
    welc_admin.setString("Welcome back, ");
    welc_admin.setOrigin(welc_admin.getLocalBounds().width / 2.f, welc_admin.getLocalBounds().height / 2.f);
    welc_admin.setPosition(width / 4.f, height / 4.f - 50.f); // (400,150) 

    // welcome text admin
    Text welc_admin_name;
    welc_admin_name.setCharacterSize(64);
    welc_admin_name.setFont(font);
    welc_admin_name.setFillColor(Color(1, 46, 90));
    welc_admin_name.setString("");
    welc_admin_name.setOrigin(welc_admin_name.getLocalBounds().width / 2.f, welc_admin_name.getLocalBounds().height / 2.f);
    welc_admin_name.setPosition(width / 4.f + 250, height / 4.f - 80.f); // (400,150) 

    Text welc_employee;
    welc_employee.setCharacterSize(64);
    welc_employee.setFont(font);
    welc_employee.setFillColor(Color(1, 46, 90));
    welc_employee.setString("Welcome back, ");
    welc_employee.setOrigin(welc_employee.getLocalBounds().width / 2.f, welc_employee.getLocalBounds().height / 2.f);
    welc_employee.setPosition(width / 2.f - 100.f, height / 4.f - 50.f); // (400,150) 

    // welcome text employee
    Text welc_employee_name;
    welc_employee_name.setCharacterSize(64);
    welc_employee_name.setFont(font);
    welc_employee_name.setFillColor(Color(1, 46, 90));
    welc_employee_name.setString("");
    welc_employee_name.setOrigin(welc_employee_name.getLocalBounds().width / 2.f, welc_employee_name.getLocalBounds().height / 2.f);
    welc_employee_name.setPosition(width / 2.f + 280.f, height / 4.f - 54.f); // (400,150) 

    // question admin
    Text question_admin;
    question_admin.setCharacterSize(42);
    question_admin.setFont(font);
    question_admin.setFillColor(Color::Black);
    question_admin.setString("What would you like to do?");
    question_admin.setOrigin(question_admin.getLocalBounds().width / 2.f, question_admin.getLocalBounds().height / 2.f);
    question_admin.setPosition(width / 4.f + 50.f, height / 4.f + 50.f); // (400,150) 

    // company name text
    Text companyName;
    companyName.setCharacterSize(36);
    companyName.setFont(font);
    companyName.setFillColor(Color::White);
    companyName.setString("2202 Group");
    companyName.setPosition(width - companyName.getLocalBounds().width - 20.f, 0.f);

    // information employee
    //TextData Emp(font, "Employee ID: \n \nFull Name: \n \nAge: \n \nPosition: \n \nPhone No.: \n \nBasic Salary: ", 36, Color(1, 46, 90), Vector2f(width/2.f-50.f, height/4.f-60.f));
    TextData IDText(font, "ID :", 20, Color::Black, Vector2f(width / 4.f * 3.f - 50, height / 4.f + 310.f));
    TextData* EmpID = new TextData[employee_count];
    for (int i = 0; i < employee_count; i++) {
        EmpID[i] = TextData(
            font,
            to_string(employee[i].id), // reuse template text
            20,
            Color::Black,
            Vector2f(width / 4.f * 3.f - 5.f, height / 4.f + 310.f) // staggered y positions
        );
    }

    TextData Deletedsuccessfully(font, "Employee Deleted Successfully", 64, Color::Red, Vector2f(width / 2.f, height / 2.f));
    Deletedsuccessfully.centerOrigin();
    TextData Addedsuccessfully(font, "Employee Added Successfully", 64, Color::Green, Vector2f(width / 2.f, height / 2.f));
    Addedsuccessfully.centerOrigin();
    TextData Recorded(font, "Attendance Recorded Successfully", 64, Color::Green, Vector2f(width / 2.f, height / 2.f));
    Recorded.centerOrigin();
    TextData Netsal(font, "Net Salary:", 36, Color(1, 46, 90), Vector2f(width / 2.f - 200.f, height / 2.f));
    Netsal.centerOrigin();
    TextData basicsal(font, "Basic Salary:", 36, Color(1, 46, 90), Vector2f(width / 4.f, height / 4.f));
    basicsal.centerOrigin();
    TextData taxsal(font, "Tax:", 36, Color(1, 46, 90), Vector2f(width / 4.f, height / 4.f+100.f));
    taxsal.centerOrigin();
    TextData bonussal(font, "Bonus:", 36, Color(1, 46, 90), Vector2f(width / 4.f, height / 4.f+200.f));
    bonussal.centerOrigin();
    TextData overtimesal(font, "OverTime Hours:", 36, Color(1, 46, 90), Vector2f(width / 4.f, height / 4.f+300.f));
    overtimesal.centerOrigin();


    TextData* EmpName = new TextData[employee_count];
    for (int i = 0; i < employee_count; i++) {
        EmpName[i] = TextData(
            font,
            employee[i].name, // reuse template text
            64,
            Color(1, 46, 90),
            Vector2f(width / 4.f - 200.f - 50, height / 4.f - 100)
        );
    }
    TextData PhoneNumberText(font, "Phone Number :", 20, Color::Black, Vector2f(width / 4.f * 3.f - 50, height / 4.f + 250.f));

    TextData* EmpPhone = new TextData[employee_count];
    for (int i = 0; i < employee_count; i++) {
        EmpPhone[i] = TextData(
            font,
            to_string(employee[i].phone), // reuse template text
            20,
            Color::Black,
            Vector2f(Vector2f(width / 4.f * 3.f + 125, height / 4.f + 250.f))
        );
    }
    TextData* EmpPosition = new TextData[employee_count];
    for (int i = 0; i < employee_count; i++) {
        EmpPosition[i] = TextData(
            font,
            employee[i].position, // reuse template text
            26,
            Color::Black,
            Vector2f(width / 4.f - 200.f - 47, height / 4.f - 14.f) // staggered y positions
        );
    }
    TextData IdNewEmployee(font, "New Employee ID IS : ", 30, Color::Black, Vector2f(width / 2.f - 180, height / 2.f + 100));
    TextData daysPresent(font, "Number Of Present Days : ", 30, Color::Black, Vector2f(width / 4.f + 220.f , height / 4.f + 70));
    daysPresent.centerOrigin(); 
    TextData daysAbsent(font, "Number Of Absent Days : ", 30, Color::Black, Vector2f(width / 4.f + 220.f, height / 4.f + 120.f));
    daysAbsent.centerOrigin();
    TextData AgeText(font, "Age :", 20, Color::Black, Vector2f(width / 4.f * 3.f - 50, height / 4.f + 370.f));
    TextData* EmpAge = new TextData[employee_count];
    for (int i = 0; i < employee_count; i++) {
        EmpAge[i] = TextData(
            font,
            to_string(employee[i].age), // reuse template text
            20,
            Color::Black,
            Vector2f(width / 4.f * 3.f + 5, height / 4.f + 370.f) // staggered y positions
        );
    }


    // seed test data
    admin[0] = { "Balona", "123" };
    admin[0].photo.loadFromFile("Images/elona.png");
    admin[0].profilePicture.setTexture(admin[0].photo);
    admin[0].profilePicture.setOrigin(Vector2f(admin[0].profilePicture.getLocalBounds().width / 2.f, admin[0].profilePicture.getLocalBounds().height / 2.f));
    admin[0].profilePicture.setPosition(Vector2f(width / 4.f * 3.f, height / 4.f + 75.f));
    admin[0].profilePicture.setScale(0.37f, 0.37f);
    admin[1] = { "Safsaf", "123" };
    admin[1].photo.loadFromFile("Images/sofia.png");
    admin[1].profilePicture.setTexture(admin[1].photo);
    admin[1].profilePicture.setOrigin(Vector2f(admin[1].profilePicture.getLocalBounds().width / 2.f, admin[1].profilePicture.getLocalBounds().height / 2.f));
    admin[1].profilePicture.setPosition(Vector2f(width / 4.f * 3.f, height / 4.f + 75.f));
    admin[1].profilePicture.setScale(0.32f, 0.32f);
    admincount = 2;
    int loggedInEmployeeIndex = -1;
    int exitcounter = 0;
    // ================================ GAME LOOP ============================
    while (window.isOpen())
    {
        Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // ========================== MENU =================================
            if (currentState == Menu)
            {
                // admin settings
                adminButton.mButton->getButtonStatus(window, event); // button pressed, hovered, etc..
                if (adminButton.mButton->isPressed)
                {
                    currentState = adminLogin; // opens admin panel
                }
                else if (adminButton.mButton->isHover) // what happens when hover
                {
                    adminButton.mButton->setButtonColor(adminButton.hoverColor);
                    adminButton.mButton->setLabelColor(Color(1, 46, 90));
                }
                else
                {
                    adminButton.mButton->setButtonColor(adminButton.defaultColor);
                    adminButton.mButton->setLabelColor(Color::White);
                }
                // admin settings end

                // employee settings
                employeeButton.mButton->getButtonStatus(window, event); // button pressed, hovered, etc..
                if (employeeButton.mButton->isPressed)
                {
                    currentState = employeeLogin; // opens employee login
                }
                else if (employeeButton.mButton->isHover) // what happens when hover
                {
                    employeeButton.mButton->setButtonColor(employeeButton.hoverColor);
                    employeeButton.mButton->setLabelColor(Color(1, 46, 90));
                }
                else
                {
                    employeeButton.mButton->setButtonColor(employeeButton.defaultColor);
                    employeeButton.mButton->setLabelColor(Color::White);
                }
                // employee settings end

                // exit settings
                exitButton.mButton->getButtonStatus(window, event);
                if (exitButton.mButton->isPressed)  // pressing exit button closes program
                {
                    window.close();
                }
                else if (exitButton.mButton->isHover)
                    exitButton.mButton->button.setFillColor(Color(255, 255, 255, 180));
                else
                    exitButton.mButton->button.setFillColor(Color::White);
                // exit settings end
            }

            // back settings
            backButton.mButton->getButtonStatus(window, event);
            if (backButton.mButton->isPressed) {
                if (currentState == adminLogin || currentState == employeeLogin) {
                    Showerror = false;
                    idBox.clear();  // added these two lines so when back is pressed boxes are cleared
                    passwordBox.clear();
                    idBoxEmp.clear();
                    passwordBoxEmp.clear();
                    currentState = Menu;
                }
            }
            else if (backButton.mButton->isHover) {
                backButton.mButton->button.setFillColor(Color(255, 255, 255, 180));
            }
            else
                backButton.mButton->button.setFillColor(Color::White);
            // back settings end

             // ================================ LOG IN ADMIN =============================
            if (currentState == adminLogin) {
                idBox.handleEvent(event, window);
                passwordBox.handleEvent(event, window);
                // log in settings
                if (currentState == adminLogin) {
                    loginButton.mButton->getButtonStatus(window, event);
                    if (loginButton.mButton->isPressed) {
                        Showerror = false;
                        if (idBox.input.empty() || passwordBox.input.empty()) {
                            Showerror = true;
                        }
                        else if (validateAdmin(idBox.input, passwordBox.input)) {
                            currentState = adminPanel;
                        }
                        else {
                            Showerror = true;
                            emptyloginbox.setString("Wrong ID or Password!");
                            exitcounter++;
                            if (exitcounter == 3) {
                                currentState = zerotrailsleft;
                            }
                        }
                    }
                    else if (loginButton.mButton->isHover) {
                        loginButton.mButton->setButtonColor(loginButton.hoverColor);
                        loginButton.mButton->setLabelColor(Color(1, 46, 90));
                    }
                    else {
                        loginButton.mButton->setButtonColor(loginButton.defaultColor);
                        loginButton.mButton->setLabelColor(Color::White);
                    }
                }
            }

            // =============================== LOG IN EMPLOYEE ============================
                // log in settings
            if (currentState == employeeLogin) {
                idBoxEmp.handleEvent(event, window);
                passwordBoxEmp.handleEvent(event, window);
                loginButton.mButton->getButtonStatus(window, event);
                if (loginButton.mButton->isPressed) {
                    Showerror = false;
                    if (idBoxEmp.input.empty() || passwordBoxEmp.input.empty()) {
                        Showerror = true;
                        emptyloginbox.setString("Fields cannot be empty!");
                    }
                    else {
                        try {
                            int foundIndex = -1;
                            if (validateEmployee(stoi(idBoxEmp.input), passwordBoxEmp.input, foundIndex, employee_count)) {
                                loggedInEmployeeIndex = foundIndex;
                                currentState = employeePanel;
                            }
                            else {
                                Showerror = true;
                                emptyloginbox.setString("Wrong ID or Password!");
                            }
                        }
                        catch (...) {
                            Showerror = true;
                            emptyloginbox.setString("ID must be a number!");
                        }
                    }
                }
                else if (loginButton.mButton->isHover) {
                    loginButton.mButton->setButtonColor(loginButton.hoverColor);
                    loginButton.mButton->setLabelColor(Color(1, 46, 90));
                }
                else {
                    loginButton.mButton->setButtonColor(loginButton.defaultColor);
                    loginButton.mButton->setLabelColor(Color::White);
                }
            }
            //log in settings end
        // ============================ ADMIN PANEL ============================
            if (currentState == adminPanel) {
                employeeidadminpanel.handleEvent(event, window);
                // add employee settings
                addButton.mButton->getButtonStatus(window, event);
                if (addButton.mButton->isPressed) {
                    currentState = addEmployeePanel;
                }
                else if (addButton.mButton->isHover) {
                    addButton.mButton->setButtonColor(addButton.hoverColor);
                    addButton.mButton->setLabelColor(Color(1, 46, 90));
                }
                else
                {
                    addButton.mButton->setButtonColor(addButton.defaultColor);
                    addButton.mButton->setLabelColor(Color::White);
                }
                // add employee settings end

                // log out settings
                logoutButton.mButton->getButtonStatus(window, event);
                if (logoutButton.mButton->isPressed) {
                    idBox.clear();
                    passwordBox.clear();
                    currentState = Menu;
                }
                else if (logoutButton.mButton->isHover) {
                    logoutButton.mButton->setLabelColor(Color::Red);
                }
                else {
                    logoutButton.mButton->setLabelColor(Color::White);
                }
                // log out settings end
                Showerror = false;
                // enter settings
                enterButton.mButton->getButtonStatus(window, event);
                if (enterButton.mButton->isPressed) {
                    if (validateid(employeeidadminpanel.input ,employee_count)) {
                        currentState = editEmployeePanel;
                    }
                    else {
                        Showerror = true;
                        emptyloginbox.setPosition(230, 500);
                        emptyloginbox.setString("Invalid ID");
                    }

                }
                else if (enterButton.mButton->isHover) {
                    enterButton.mButton->setButtonColor(enterButton.hoverColor);
                    enterButton.mButton->setLabelColor(Color(1, 46, 90));
                }
                else
                {
                    enterButton.mButton->setButtonColor(enterButton.defaultColor);
                    enterButton.mButton->setLabelColor(Color::White);
                }
                // end enter settings
            }
            // ====================== EDIT EMPLOYEE IN ADMIN PANEL ==========================
            else if (currentState == editEmployeePanel) {
                // update settings
                updateButton.mButton->getButtonStatus(window, event);
                if (updateButton.mButton->isPressed) {
                    currentState = updatePanel;
                }
                else if (updateButton.mButton->isHover) {
                    updateButton.mButton->setButtonColor(updateButton.hoverColor);
                    updateButton.mButton->setLabelColor(Color(1, 46, 90));
                }
                else
                {
                    updateButton.mButton->setButtonColor(updateButton.defaultColor);
                    updateButton.mButton->setLabelColor(Color::White);
                }
                // update settings end

                // back settings
                backButton.mButton->getButtonStatus(window, event);
                if (backButton.mButton->isPressed) {
                    if (currentState == editEmployeePanel) {
                        employeeidadminpanel.clear();
                        currentState = adminPanel;
                    }
                }
                else if (backButton.mButton->isHover) {
                    backButton.mButton->button.setFillColor(Color(255, 255, 255, 180));
                }
                else
                    backButton.mButton->button.setFillColor(Color::White);
                // back settings end

                // attendance settings
                attendanceButton.mButton->getButtonStatus(window, event);
                if (attendanceButton.mButton->isPressed) {
                    currentState = attendanceEditPanel;
                }
                else if (attendanceButton.mButton->isHover) {
                    attendanceButton.mButton->setButtonColor(attendanceButton.hoverColor);
                    attendanceButton.mButton->setLabelColor(Color(1, 46, 90));
                }
                else
                {
                    attendanceButton.mButton->setButtonColor(attendanceButton.defaultColor);
                    attendanceButton.mButton->setLabelColor(Color::White);
                }
                // attendance settings end

                // calc salary settings
                salaryButton.mButton->getButtonStatus(window, event);
                if (salaryButton.mButton->isPressed) {
                    currentState = salaryCalcPanel;
                }
                else if (salaryButton.mButton->isHover) {
                    salaryButton.mButton->setButtonColor(salaryButton.hoverColor);
                    salaryButton.mButton->setLabelColor(Color(1, 46, 90));
                }
                else
                {
                    salaryButton.mButton->setButtonColor(salaryButton.defaultColor);
                    salaryButton.mButton->setLabelColor(Color::White);
                }
                // calc salary settings end

                // delete employee settings
                deleteButton.mButton->getButtonStatus(window, event);
                if (deleteButton.mButton->isPressed) {
                    deleteEmployee(stoi(employeeidadminpanel.input), employee_count);
                    rebuildData(EmpID, EmpName, EmpPhone, EmpPosition, EmpAge, employee_count, font, width, height);
                    employeeidadminpanel.clear();
                    currentState = deletePanel;
                }
                else if (deleteButton.mButton->isHover) {
                    deleteButton.mButton->setButtonColor(deleteButton.hoverColor);
                    deleteButton.mButton->setLabelColor(Color::Red);
                }
                else
                {
                    deleteButton.mButton->setButtonColor(deleteButton.defaultColor);
                    deleteButton.mButton->setLabelColor(Color::White);
                }
                // delete employee settings end
            }
            // ======================== EMPLOYEE PANEL =========================
            if (currentState == employeePanel) {
                // view settings
                viewButton.mButton->getButtonStatus(window, event);
                if (viewButton.mButton->isPressed) {
                    currentState = viewPanel;
                }
                else if (viewButton.mButton->isHover) {
                    viewButton.mButton->setButtonColor(viewButton.hoverColor);
                    viewButton.mButton->setLabelColor(Color(1, 46, 90));
                }
                else
                {
                    viewButton.mButton->setButtonColor(viewButton.defaultColor);
                    viewButton.mButton->setLabelColor(Color::White);
                }
                // view settings end

                // log out settings
                logoutButton.mButton->getButtonStatus(window, event);
                if (logoutButton.mButton->isPressed) {
                    idBoxEmp.clear();
                    passwordBoxEmp.clear();
                    currentState = Menu;
                }
                else if (logoutButton.mButton->isHover) {
                    logoutButton.mButton->setLabelColor(Color::Red);
                }
                else {
                    logoutButton.mButton->setLabelColor(Color::White);
                }
                // log out settings end

                // view salary settings
                viewSalaryButton.mButton->getButtonStatus(window, event);
                if (viewSalaryButton.mButton->isPressed) {
                    currentState = salaryPanel;
                }
                else if (viewSalaryButton.mButton->isHover) {
                    viewSalaryButton.mButton->setButtonColor(viewSalaryButton.hoverColor);
                    viewSalaryButton.mButton->setLabelColor(Color(1, 46, 90));
                }
                else
                {
                    viewSalaryButton.mButton->setButtonColor(viewSalaryButton.defaultColor);
                    viewSalaryButton.mButton->setLabelColor(Color::White);
                }
                // view salary settings end 

                // view attendance settings
                viewAttendButton.mButton->getButtonStatus(window, event);
                if (viewAttendButton.mButton->isPressed) {
                    currentState = attendancePanel;
                }
                else if (viewAttendButton.mButton->isHover) {
                    viewAttendButton.mButton->setButtonColor(viewAttendButton.hoverColor);
                    viewAttendButton.mButton->setLabelColor(Color(1, 46, 90));
                }
                else
                {
                    viewAttendButton.mButton->setButtonColor(viewAttendButton.defaultColor);
                    viewAttendButton.mButton->setLabelColor(Color::White);
                }
                // view attendance settings end 
            }
            // ================= EDIT ATTENDANCE PANEL =======================
            else if (currentState == attendanceEditPanel) {
                // back settings
                backButton.mButton->getButtonStatus(window, event);
                if (backButton.mButton->isPressed) {
                    if (currentState == attendanceEditPanel) {
                        currentState = editEmployeePanel;
                    }
                }
                else if (backButton.mButton->isHover) {
                    backButton.mButton->button.setFillColor(Color(255, 255, 255, 180));
                }
                else
                    backButton.mButton->button.setFillColor(Color::White);
                // back settings end

                monthBox.handleEvent(event, window);
                presentBox.handleEvent(event, window);
                absentBox.handleEvent(event, window);

                enterOkButton.mButton->getButtonStatus(window, event);
                if (enterOkButton.mButton->isPressed) {
                    Showerror = false;
                    if (monthBox.input.empty() || presentBox.input.empty() || absentBox.input.empty()) {
                        Showerror = true;
                        emptyloginbox.setPosition(width / 4.f, height / 4.f + 300.f);
                        emptyloginbox.setString("Fields cannot be empty!");
                    }
                    else {
                        try {
                            int month = stoi(monthBox.input);
                            if (month < 1 || month > 12) {
                                Showerror = true;
                                emptyloginbox.setPosition(width / 4.f, height / 4.f + 300.f);
                                emptyloginbox.setString("Invalid Month! Enter 1-12");
                                monthBox.clear();
                            }
                            else {
                                currentState = attendanceOkPanel;
                                manageAttendance(stoi(employeeidadminpanel.input), stoi(presentBox.input), stoi(absentBox.input));
                            }
                        }
                        catch (...) {
                            Showerror = true;
                            emptyloginbox.setString("Month must be a number!");
                        }
                    }
                }
                else if (enterOkButton.mButton->isHover) {
                    enterOkButton.mButton->setButtonColor(enterOkButton.hoverColor);
                    enterOkButton.mButton->setLabelColor(Color(1, 46, 90));
                }
                else
                {
                    enterOkButton.mButton->setButtonColor(enterOkButton.defaultColor);
                    enterOkButton.mButton->setLabelColor(Color::White);
                }

            }
            // ====================== DELETE PANEL =======================
            else if (currentState == deletePanel) {
                // okay button settings
                deleteButtonOkay.mButton->getButtonStatus(window, event);
                if (deleteButtonOkay.mButton->isPressed) {
                    currentState = adminPanel;
                    employeeidadminpanel.clear();
                }
                else if (deleteButtonOkay.mButton->isHover) {
                    deleteButtonOkay.mButton->setButtonColor(deleteButtonOkay.hoverColor);
                    deleteButtonOkay.mButton->setLabelColor(Color(1, 46, 90));
                }
                else
                {
                    deleteButtonOkay.mButton->setButtonColor(deleteButtonOkay.defaultColor);
                    deleteButtonOkay.mButton->setLabelColor(Color::White);
                }
                // okay button settings end
            }
            // ========================= ATTENDANCE RECORDED PANEL =======================
            else if (currentState == attendanceOkPanel) {
                deleteButtonOkay.mButton->getButtonStatus(window, event);
                if (deleteButtonOkay.mButton->isPressed) {
                    currentState = adminPanel;
                }
                else if (deleteButtonOkay.mButton->isHover) {
                    deleteButtonOkay.mButton->setButtonColor(deleteButtonOkay.hoverColor);
                    deleteButtonOkay.mButton->setLabelColor(Color(1, 46, 90));
                }
                else
                {
                    deleteButtonOkay.mButton->setButtonColor(deleteButtonOkay.defaultColor);
                    deleteButtonOkay.mButton->setLabelColor(Color::White);
                }
            }
            //============================ addEmployeePanel =============================
            else if (currentState == addEmployeePanel) {
                NameBox.handleEvent(event, window);
                PassBox.handleEvent(event, window);
                AgeBox2.handleEvent(event, window);
                PositionBox.handleEvent(event, window);
                PhoneNumberBox.handleEvent(event, window);
                BasicSalBox.handleEvent(event, window);
                enterOkButton.mButton->getButtonStatus(window, event);
                if (enterOkButton.mButton->isPressed) {
                    Showerror = false;
                    if (NameBox.input.empty() || PassBox.input.empty() || AgeBox2.input.empty()) {
                        Showerror = true;
                        emptyloginbox.setPosition(width / 4.f, height / 4.f + 300.f);
                        emptyloginbox.setString("Fields cannot be empty!");
                    }
                    else {
                        addEmployee(employee_count, NameBox.input, stoi(AgeBox2.input), PositionBox.input, stoll(PhoneNumberBox.input), PassBox.input, stoi(BasicSalBox.input));      
                        rebuildData(EmpID, EmpName, EmpPhone, EmpPosition, EmpAge, employee_count, font, width, height);
                        currentState = addedsuccessfully;
                        
                    }
                }
                else if (enterOkButton.mButton->isHover) {
                    enterOkButton.mButton->setButtonColor(enterOkButton.hoverColor);
                    enterOkButton.mButton->setLabelColor(Color(1, 46, 90));
                }
                else
                {
                    enterOkButton.mButton->setButtonColor(enterOkButton.defaultColor);
                    enterOkButton.mButton->setLabelColor(Color::White);
                }

            }
            // ======================== VIEW ATTENDANCE AS EMPLOYEE======================
            else if (currentState == attendancePanel) {
                monthBox.handleEvent(event, window);
                enterOkButton.mButton->getButtonStatus(window, event);
                if (enterOkButton.mButton->isPressed) {
                    Showerror = false;
                    if (monthBox.input.empty()) {
                        Showerror = true;
                        emptyloginbox.setPosition(width / 4.f, height / 4.f + 300.f);
                        emptyloginbox.setString("Fields cannot be empty!");
                    }
                    else {
                        try {
                            int month = stoi(monthBox.input);
                            if (month < 1 || month > 12) {
                                Showerror = true;
                                emptyloginbox.setPosition(width / 4.f, height / 4.f + 300.f);
                                emptyloginbox.setString("Invalid Month! Enter 1-12");
                                monthBox.clear();
                            }
                            else {
                                currentState = attendanceViewPanel;
                            }
                        }
                        catch (...) {
                            Showerror = true;
                            emptyloginbox.setString("Month must be a number!");
                        }
                    }
                }
                else if (enterOkButton.mButton->isHover) {
                    enterOkButton.mButton->setButtonColor(enterOkButton.hoverColor);
                    enterOkButton.mButton->setLabelColor(Color(1, 46, 90));
                }
                else
                {
                    enterOkButton.mButton->setButtonColor(enterOkButton.defaultColor);
                    enterOkButton.mButton->setLabelColor(Color::White);
                }
                // back settings
                backButton.mButton->getButtonStatus(window, event);
                if (backButton.mButton->isPressed) {
                    if (currentState == attendancePanel) {
                        currentState = employeePanel;
                    }
                }
                else if (backButton.mButton->isHover) {
                    backButton.mButton->button.setFillColor(Color(255, 255, 255, 180));
                }
                else
                    backButton.mButton->button.setFillColor(Color::White);
                // back settings end
            }
            //===========================attendanceViewPanel==============================
            else if (currentState == attendanceViewPanel) {
                deleteButtonOkay.mButton->getButtonStatus(window, event);
                if (deleteButtonOkay.mButton->isPressed) {
                    if (currentState == attendanceViewPanel) {
                        currentState = employeePanel;
                    }
                }
                else if (deleteButtonOkay.mButton->isHover) {
                    deleteButtonOkay.mButton->setButtonColor(deleteButtonOkay.hoverColor);
                    deleteButtonOkay.mButton->setLabelColor(Color(1, 46, 90));
                }
                else
                {
                    deleteButtonOkay.mButton->setButtonColor(deleteButtonOkay.defaultColor);
                    deleteButtonOkay.mButton->setLabelColor(Color::White);
                }
            }
            // ======================== CALCULATE SALARY PANEL ==========================
            else if (currentState == salaryCalcPanel) {
                basicSalBox.handleEvent(event, window);
                bonusBox.handleEvent(event, window);
                overtimeBox.handleEvent(event, window);
                deductionBox.handleEvent(event, window);
                monthBox.handleEvent(event, window);

                enterOkButton.mButton->getButtonStatus(window, event);
                if (enterOkButton.mButton->isPressed) {
                    Showerror = false;
                    if (basicSalBox.input.empty() || bonusBox.input.empty() || overtimeBox.input.empty() || monthBox.input.empty()) {
                        Showerror = true;
                        emptyloginbox.setString("Fields cannot be empty!");
                    }
                    else {
                        try {
                            int empID = stoi(employeeidadminpanel.input);
                            int month = stoi(monthBox.input);
                            float net = calcSalary(empID, basicSalBox.input,
                                bonusBox.input, overtimeBox.input, month, deductionBox.input);
                            if (net == -1) {
                                Showerror = true;
                                emptyloginbox.setString("Employee not found!");
                            }
                            else if (month < 1 || month > 12) {
                                Showerror = true;
                                emptyloginbox.setString("Invalid Month! Enter 1-12");
                                monthBox.clear();
                            }
                            else {
                                calculatedNetSalary = net;
                                currentState = netSalaryPanel;
                            }
                        }
                        catch (...) {
                            Showerror = true;
                            emptyloginbox.setString("Invalid input!");
                        }
                    }

                }
                else if (enterOkButton.mButton->isHover) {
                    enterOkButton.mButton->setButtonColor(enterOkButton.hoverColor);
                    enterOkButton.mButton->setLabelColor(Color(1, 46, 90));
                }
                else
                {
                    enterOkButton.mButton->setButtonColor(enterOkButton.defaultColor);
                    enterOkButton.mButton->setLabelColor(Color::White);

                }
                // back settings
                backButton.mButton->getButtonStatus(window, event);
                if (backButton.mButton->isPressed) {
                    if (currentState == salaryCalcPanel) {
                        currentState = editEmployeePanel;
                    }
                }
                else if (backButton.mButton->isHover) {
                    backButton.mButton->button.setFillColor(Color(255, 255, 255, 180));
                }
                else
                    backButton.mButton->button.setFillColor(Color::White);
                // back settings end

            }
            // ============ NET SALARY PANEL ================
            else if (currentState == netSalaryPanel) {
                // okay button settings
                deleteButtonOkay.mButton->getButtonStatus(window, event);
                if (deleteButtonOkay.mButton->isPressed) {
                    currentState = adminPanel;
                    employeeidadminpanel.clear();
                }
                else if (deleteButtonOkay.mButton->isHover) {
                    deleteButtonOkay.mButton->setButtonColor(deleteButtonOkay.hoverColor);
                    deleteButtonOkay.mButton->setLabelColor(Color(1, 46, 90));
                }
                else
                {
                    deleteButtonOkay.mButton->setButtonColor(deleteButtonOkay.defaultColor);
                    deleteButtonOkay.mButton->setLabelColor(Color::White);
                }
                // okay button settings end
            }
            // =================== SALARY VIEW PANEL ================
            else if (currentState == salaryPanel) {
                monthBox.handleEvent(event, window);
                enterOkButton.mButton->getButtonStatus(window, event);
                if (enterOkButton.mButton->isPressed) {
                    Showerror = false;
                    if (monthBox.input.empty()) {
                        Showerror = true;
                        emptyloginbox.setPosition(width / 4.f, height / 4.f + 300.f);
                        emptyloginbox.setString("Fields cannot be empty!");
                    }
                    else {
                        try {
                            int month = stoi(monthBox.input);
                            if (month < 1 || month > 12) {
                                Showerror = true;
                                emptyloginbox.setPosition(width / 4.f, height / 4.f + 300.f);
                                emptyloginbox.setString("Invalid Month! Enter 1-12");
                                monthBox.clear();
                            }
                            else {
                                currentState = salaryViewPanel;
                            }
                        }
                        catch (...) {
                            Showerror = true;
                            emptyloginbox.setString("Month must be a number!");
                        }
                    }
                }
                else if (enterOkButton.mButton->isHover) {
                    enterOkButton.mButton->setButtonColor(enterOkButton.hoverColor);
                    enterOkButton.mButton->setLabelColor(Color(1, 46, 90));
                }
                else
                {
                    enterOkButton.mButton->setButtonColor(enterOkButton.defaultColor);
                    enterOkButton.mButton->setLabelColor(Color::White);
                }


            }
            // ======================= SALARY VIEW PANEL 2 ========================
            else if (currentState == salaryViewPanel) {

                deleteButtonOkay.mButton->getButtonStatus(window, event);
                if (deleteButtonOkay.mButton->isPressed) {
                    if (currentState == salaryViewPanel) {
                        currentState = employeePanel;
                    }

                }
                else if (deleteButtonOkay.mButton->isHover) {
                    deleteButtonOkay.mButton->setButtonColor(deleteButtonOkay.hoverColor);
                    deleteButtonOkay.mButton->setLabelColor(Color(1, 46, 90));
                }
                else {
                    deleteButtonOkay.mButton->setButtonColor(deleteButtonOkay.defaultColor);
                    deleteButtonOkay.mButton->setLabelColor(Color::White);
                }
            }
            // =============== ADDED SUCCESSFULLY==========
            else if (currentState == addedsuccessfully) {
                deleteButtonOkay.mButton->getButtonStatus(window, event);
                if (deleteButtonOkay.mButton->isPressed) {
                    if (currentState == addedsuccessfully) {
                        currentState = adminPanel;
                    }

                }
                else if (deleteButtonOkay.mButton->isHover) {
                    deleteButtonOkay.mButton->setButtonColor(deleteButtonOkay.hoverColor);
                    deleteButtonOkay.mButton->setLabelColor(Color(1, 46, 90));
                }
                else {
                    deleteButtonOkay.mButton->setButtonColor(deleteButtonOkay.defaultColor);
                    deleteButtonOkay.mButton->setLabelColor(Color::White);
                }

            }

        }
    // Draw
window.clear(Color::White); // white background

if (currentState == Menu)
{
    window.setTitle("Employee Payroll Management System By 2202 GROUP"); //added this so that title is set to this when we are in main menu 
    window.draw(topBar);
    window.draw(companyName);
    adminButton.mButton->draw(window);
    employeeButton.mButton->draw(window);
    exitButton.mButton->draw(window);
    window.draw(adminSprite);
    window.draw(employeeSprite);
    window.draw(welc);
}
else if (currentState == adminLogin) {
    window.setTitle("Admin Log In");
    window.draw(adminImageSprite);
    window.draw(topBar);
    window.draw(companyName);
    idBox.draw(window);
    passwordBox.draw(window);
    backButton.mButton->draw(window);
    loginButton.mButton->draw(window);
    if (Showerror == true)
        window.draw(emptyloginbox);
}

else if (currentState == zerotrailsleft) {
    topBar.setFillColor(Color :: Red);
    window.setTitle("NUMBER OF TRAILS EXCEEDED");
    window.draw(adminImageSprite);
    window.draw(topBar);
    window.draw(companyName);
    exitAppButton.mButton->draw(window);
    exitAppButton.mButton->getButtonStatus(window, event);
    if (exitAppButton.mButton->isPressed) {
        window.close();
    }
}
else if (currentState == employeeLogin) {
    window.setTitle("Employee Log In");
    window.draw(employeeImageSprite);
    window.draw(topBar);
    window.draw(companyName);
    idBoxEmp.draw(window);
    passwordBoxEmp.draw(window);
    backButton.mButton->draw(window);
    loginButton.mButton->draw(window);
    if (Showerror == true)
        window.draw(emptyloginbox);
}
else if (currentState == adminPanel) {
    window.setTitle("ADMIN PANEL");
    window.draw(adminImageSprite);
    window.draw(topBar);
    window.draw(companyName);
    addButton.mButton->draw(window);
    window.draw(welc_admin);
    welc_admin_name.setString(idBox.input);
    window.draw(welc_admin_name);
    window.draw(question_admin);
    logoutButton.mButton->draw(window);
    enterButton.mButton->draw(window);
    employeeidadminpanel.draw(window);
    employeeidadminpanel.draw(window);
    if (Showerror == true)
        window.draw(emptyloginbox);
    if (idBox.input == admin[0].username && passwordBox.input == admin[0].password) {
        window.draw(admin[0].profilePicture);
    }
    else if (idBox.input == admin[1].username && passwordBox.input == admin[1].password)
        window.draw(admin[1].profilePicture);

}
else if (currentState == employeePanel) {
    window.setTitle("EMPLOYEE PANEL");
    window.draw(adminImageSprite);
    window.draw(topBar);
    window.draw(companyName);
    viewButton.mButton->draw(window);
    logoutButton.mButton->draw(window);
    viewSalaryButton.mButton->draw(window);
    viewAttendButton.mButton->draw(window);
    window.draw(welc_employee);
    if (loggedInEmployeeIndex != -1) {
        int i = loggedInEmployeeIndex;
        welc_employee_name.setString(employee[i].name);
        welc_employee_name.setOrigin(welc_employee_name.getLocalBounds().width / 2.f, welc_employee_name.getLocalBounds().height / 2.f);
        window.draw(welc_employee_name);
    }
}
else if (currentState == editEmployeePanel) {
    window.setTitle("EDIT EMPLOYEE");
    window.draw(adminImageSprite);
    window.draw(topBar);
    window.draw(companyName);
    updateButton.mButton->draw(window);
    backButton.mButton->draw(window);
    attendanceButton.mButton->draw(window);
    salaryButton.mButton->draw(window);
    deleteButton.mButton->draw(window);
    window.draw(PhoneNumberText.text);
    window.draw(IDText.text);
    window.draw(AgeText.text);
    try {
        int searchID = stoi(employeeidadminpanel.input);
        for (int i = 0; i < employee_count; i++) {
            if (searchID == employee[i].id) {              // draw field labels
                window.draw(EmpAge[i].text);
                window.draw(EmpName[i].text);       // draw name
                window.draw(EmpID[i].text);         // draw ID
                window.draw(EmpPhone[i].text);
                // draw phone
                window.draw(EmpPosition[i].text);   // draw position
                window.draw(employee[i].profilePicture);
            }
        }
    }
    catch (...) {} // silently ignore if input isn't a number yet
}
else if (currentState == attendanceEditPanel) {
    window.setTitle("EDIT ATTENDANCE");
    window.draw(adminImageSprite);
    window.draw(topBar);
    window.draw(companyName);
    monthBox.draw(window);
    presentBox.draw(window);
    absentBox.draw(window);
    if (Showerror == true) {
        window.draw(emptyloginbox);
    }
    enterOkButton.mButton->draw(window);
}
else if (currentState == attendanceViewPanel) {
    window.setTitle("View Your Attendance");
    window.draw(adminImageSprite);
    window.draw(topBar);
    window.draw(companyName);
    monthBox.draw(window);
    deleteButtonOkay.mButton->draw(window);
    for (int i = 0; i < employee_count; i++) {
        if (stoi(idBoxEmp.input) == employee[i].id) {
            daysPresent.text.setString("Days Present : " + to_string(employee[i].attendance.dayspresent));
            window.draw(daysPresent.text);
            daysAbsent.text.setString("Days Absent : " + to_string(employee[i].attendance.daysabsent));
            window.draw(daysAbsent.text);

        }
    }
    }
else if (currentState == deletePanel) {
    window.setTitle("DELETED SUCCESSFULLY!");
    window.draw(adminImageSprite);
    window.draw(topBar);
    window.draw(companyName);
    deleteButtonOkay.mButton->draw(window);
    window.draw(Deletedsuccessfully.text);

}
else if (currentState == attendanceOkPanel) {
    window.setTitle("RECORDED SUCCESSFULLY!");
    window.draw(adminImageSprite);
    window.draw(topBar);
    window.draw(companyName);
    deleteButtonOkay.mButton->draw(window);
    window.draw(Recorded.text);
}
else if (currentState == addEmployeePanel) {
    window.setTitle("Add Employee");
    window.draw(adminImageSprite);
    window.draw(topBar);
    window.draw(companyName);
    NameBox.draw(window);
    AgeBox2.draw(window);
    PositionBox.draw(window);
    PhoneNumberBox.draw(window);
    PassBox.draw(window);
    BasicSalBox.draw(window);
    enterOkButton.mButton->draw(window);

  

}
else if (currentState == addedsuccessfully) {
    window.setTitle("ADDED SUCCESSFULLY");
    window.draw(adminImageSprite);
    window.draw(topBar);
    window.draw(companyName);
    window.draw(Addedsuccessfully.text);
    IdNewEmployee.text.setString("New ID Employee Is : " + to_string(employee[employee_count-1].id));
    window.draw(IdNewEmployee.text);
    deleteButtonOkay.mButton->draw(window);
}
else if (currentState == viewPanel) {
    window.setTitle("View Your Information");
    window.draw(adminImageSprite);
    window.draw(topBar);
    backButton.mButton->draw(window);
    window.draw(companyName);
    try {
        int searchEmpID = stoi(idBoxEmp.input);
        for (int i = 0; i < employee_count; i++) {
            if (searchEmpID == employee[i].id) {              // draw field labels
                window.draw(EmpName[i].text);       // draw name
                window.draw(EmpID[i].text);         // draw ID
                window.draw(EmpPhone[i].text);
                window.draw(EmpPosition[i].text);
                window.draw(PhoneNumberText.text);
                window.draw(IDText.text);
                window.draw(EmpAge[i].text);
                window.draw(AgeText.text);
                window.draw(EmpPosition[i].text);   // draw position
                window.draw(employee[i].profilePicture);
            }
        }
    }
    catch (...) {} // silently ignore if input isn't a number yet
    if (backButton.mButton->isPressed) {
        if (currentState == viewPanel) {
            backButton.mButton->getButtonStatus(window, event);
            currentState = employeePanel;
        }
    }
    logoutButton.mButton->getButtonStatus(window, event);
    if (logoutButton.mButton->isPressed) {
        currentState = Menu;
    }
    else if (logoutButton.mButton->isHover) {
        logoutButton.mButton->setLabelColor(Color::Red);
    }
    else {
        logoutButton.mButton->setLabelColor(Color::White);
    }
}
else if (currentState == salaryCalcPanel) {
    window.setTitle("CALCULATE SALARY");
    window.draw(adminImageSprite);
    window.draw(topBar);
    window.draw(companyName);
    basicSalBox.draw(window);
    bonusBox.draw(window);
    overtimeBox.draw(window);
    deductionBox.draw(window);
    monthBox.draw(window);
    enterOkButton.mButton->draw(window);
    backButton.mButton->draw(window);
}
else if (currentState == attendancePanel) {
    window.setTitle("View Attendance Records");
    window.draw(adminImageSprite);
    window.draw(topBar);
    window.draw(companyName);
    monthBox.draw(window);
    if (Showerror == true) {
        window.draw(emptyloginbox);
    }
    enterOkButton.mButton->draw(window);
    backButton.mButton->draw(window);
}
else if (currentState == netSalaryPanel) {
    window.setTitle("NET SALARY");
    window.draw(adminImageSprite);
    window.draw(topBar);
    window.draw(companyName);
    Netsal.text.setString("Net Salary: " + to_string(calculatedNetSalary));
    window.draw(Netsal.text);
    deleteButtonOkay.mButton->draw(window);
}
else if (currentState == salaryPanel) {
    window.setTitle("VIEW SALARY");
    window.draw(adminImageSprite);
    window.draw(topBar);
    window.draw(companyName);
    enterOkButton.mButton->draw(window);
    monthBox.draw(window);
    if (Showerror == true)
        window.draw(emptyloginbox);
}
else if (currentState == salaryViewPanel) {
    window.setTitle("SALARY VIEW CONT.");
    window.draw(adminImageSprite);
    window.draw(topBar);
    window.draw(companyName);
    for (int i = 0; i < employee_count; i++) {
        if (stoi(idBoxEmp.input) == employee[i].id) {
            basicsal.text.setString("Basic Salary : " + to_string(employee[i].basicsalary));
            window.draw(basicsal.text);
            taxsal.text.setString("Tax : " + to_string(employee[i].tax));
            window.draw(taxsal.text);
            bonussal.text.setString("Bonus : " + to_string(employee[i].bonus));
            overtimesal.text.setString("Overtime : " + to_string(employee[i].overtimehrs));
            Netsal.text.setString("Net Salary: " + to_string(calculatedNetSalary));
            window.draw(Netsal.text);
            window.draw(bonussal.text);
            window.draw(overtimesal.text);
        }
    }
    deleteButtonOkay.mButton->draw(window);

}




    window.display();

    }

    

    delete adminButton.mButton;
    delete employeeButton.mButton;
    delete exitButton.mButton;
    delete backButton.mButton;
    delete loginButton.mButton;
    delete addButton.mButton;
    delete logoutButton.mButton;
    delete enterButton.mButton;
    delete updateButton.mButton;
    delete attendanceButton.mButton;
    delete salaryButton.mButton;
    delete deleteButton.mButton;
    delete viewButton.mButton;
    delete viewSalaryButton.mButton;
    delete viewAttendButton.mButton;
    delete deleteButtonOkay.mButton;
    delete enterOkButton.mButton;

    return 0;
}// closes main()