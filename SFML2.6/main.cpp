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
    employeePanel
};


struct Textboxdata {
    RectangleShape box;
    Text label;
    Text displayText;
    string input;
    bool isFocused = false;

    Color defaultOutline = Color::Black;
    Color focusedOutline = Color(31, 11, 64);

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
    }adminButton,employeeButton,exitButton,backButton, loginButton;
    

int main()
{
    GameState currentState = Menu; // screen when you open window

    unsigned int height = 800; // height of window
    unsigned int width = 1600; // width of window

    RenderWindow window(VideoMode(width, height), "Employee Payroll Management System");

    window.setFramerateLimit(60); // frame limit of window
    window.setKeyRepeatEnabled(false); // one press each time

    Font font; // blockletter font
    if (!font.loadFromFile("Fonts/Blockletter.otf")) {
        cerr << "ERROR :: COULD_NOT_OPEN_FROM_FILE :: MAIN::Fonts/Blockletter.otf" << endl;
    }

    // top of the window details
    RectangleShape topBar(Vector2f(1600, 60));
    topBar.setPosition(0, 0);
    topBar.setFillColor(Color(31, 11, 64));
    // top of the window details end

    // admin button details
    adminButton.label = "I am an admin";
    adminButton.defaultColor = Color(31, 11, 64);
    adminButton.hoverColor = Color(31, 11, 64, 185);

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
    employeeButton.label = "I am an employee";
    employeeButton.defaultColor = Color(31, 11, 64);
    employeeButton.hoverColor = Color(31, 11, 64, 185);

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
    loginButton.label = "log in";
    loginButton.defaultColor = Color(31, 11, 64);
    loginButton.hoverColor = Color(31, 11, 64, 185);

    Text login_text;
    login_text.setFont(font);
    login_text.setCharacterSize(36);
    login_text.setString(loginButton.label);
    FloatRect login_tb = login_text.getGlobalBounds();
    Vector2f login_size(login_tb.width * 1.5f, login_tb.height * 2.f);

    loginButton.mButton = new RectButton(font, login_size, Vector2f(width / 4.f * 3.f - login_size.x / 2.f, height / 4.f * 3.f - login_size.y / 2.f));
    loginButton.mButton->setButtonLabel(36, loginButton.label);
    loginButton.mButton->setLabelColor(Color::White);
    loginButton.mButton->setButtonColor(loginButton.defaultColor);
    // end log in button details

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
    adminImage.loadFromFile("Images/accountant_2.jpg");
    Sprite adminImageSprite;
    adminImageSprite.setTexture(adminImage);
    adminImageSprite.setOrigin(adminImageSprite.getLocalBounds().width / 2, adminImageSprite.getLocalBounds().height / 2);
    adminImageSprite.setPosition(width / 4.f + 50.f, height / 2.f + 35.f); // (450,435)
    adminImageSprite.setScale(0.2f, 0.2f);
    // admin login image end

    // employee login image 
    Texture employeeImage;
    employeeImage.loadFromFile("Images/43932.jpg");
    Sprite employeeImageSprite;
    employeeImageSprite.setTexture(employeeImage);
    employeeImageSprite.setOrigin(employeeImageSprite.getLocalBounds().width / 2, employeeImageSprite.getLocalBounds().height / 2);
    employeeImageSprite.setPosition(width / 4.f + 50.f, height / 2.f + 35.f); // (450,435)
    employeeImageSprite.setScale(0.5f,0.5f);
    // employee login image end

    // welcome text
    Text welc;
    welc.setCharacterSize(64);
    welc.setFont(font);
    welc.setFillColor(Color(31, 11, 64));
    welc.setString("Welcome!");
    welc.setOrigin(welc.getLocalBounds().width / 2.f, welc.getLocalBounds().height / 2.f);
    welc.setPosition(width / 2.f, height / 4.f - 50.f);

    // welcome text admin
    Text welc_admin;
    welc.setCharacterSize(64);
    welc.setFont(font);
    welc.setFillColor(Color(31, 11, 64));
    welc.setString("Welcome back!");
    welc.setOrigin(welc.getLocalBounds().width / 2.f, welc.getLocalBounds().height / 2.f);
    welc.setPosition(width / 2.f, height / 4.f - 50.f);

    // trying to make a textbox
    Textboxdata idBox(font, Vector2f(300, 40), Vector2f(1100, 250), "Enter Your ID Number:");
    Textboxdata passwordBox(font, Vector2f(300, 40), Vector2f(1100, 350), "Enter Your Password:");

    // textbox attempt end

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
                    adminButton.mButton->setLabelColor(Color::Black);
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
                    employeeButton.mButton->setLabelColor(Color::Black);
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
                if (currentState == adminLogin || currentState == employeeLogin)
                    currentState = Menu;
            }
            else if (backButton.mButton->isHover) {
                backButton.mButton->button.setFillColor(Color(255, 255, 255, 180));
            }
            else
                backButton.mButton->button.setFillColor(Color::White);
            // back settings end

             // ================================ LOG IN ADMIN ========================
            if (currentState == adminLogin) {
                // log in settings
                loginButton.mButton->getButtonStatus(window, event);
                if (loginButton.mButton->isPressed) {
                    currentState = adminPanel;
                }
                else if (loginButton.mButton->isHover) {
                    loginButton.mButton->setButtonColor(loginButton.hoverColor);
                    loginButton.mButton->setLabelColor(Color::Black);
                }
                else
                {
                    loginButton.mButton->setButtonColor(loginButton.defaultColor);
                    loginButton.mButton->setLabelColor(Color::White);
                }
                //log in settings end
            }

            // =============================== LOG IN EMPLOYEE ============================
            if (currentState == employeeLogin) {
                // log in settings
                loginButton.mButton->getButtonStatus(window, event);
                if (loginButton.mButton->isPressed) {
                    currentState = employeePanel;
                }
                else if (loginButton.mButton->isHover) {
                    loginButton.mButton->setButtonColor(loginButton.hoverColor);
                    loginButton.mButton->setLabelColor(Color::Black);
                }
                else
                {
                    loginButton.mButton->setButtonColor(loginButton.defaultColor);
                    loginButton.mButton->setLabelColor(Color::White);
                }
                //log in settings end
            }

            if (currentState == adminLogin)
                idBox.handleEvent(event, window);
                passwordBox.handleEvent(event, window);
    }
    // Update

           // Draw
        window.clear(Color::White); // white background

    if (currentState == Menu)
    {
        window.draw(topBar);
        adminButton.mButton->draw(window);
        employeeButton.mButton->draw(window);
        exitButton.mButton->draw(window);
        window.draw(adminSprite);
        window.draw(employeeSprite);
        window.draw(welc);
    }
    else if (currentState == adminLogin) {
        window.draw(adminImageSprite);
        idBox.draw(window);
        passwordBox.draw(window);
        window.draw(topBar);
        backButton.mButton->draw(window);
        loginButton.mButton->draw(window);
    }
    else if (currentState == employeeLogin) {
        window.draw(topBar);
        backButton.mButton->draw(window);
        loginButton.mButton->draw(window);
    }
    else if (currentState == adminPanel) {
        window.draw(topBar);
    }
    else if (currentState == employeePanel) {
        window.draw(topBar);
    }
    window.display();
        }
        delete adminButton.mButton;
        delete employeeButton.mButton;
        delete exitButton.mButton;
        delete backButton.mButton;
        delete loginButton.mButton;
} 