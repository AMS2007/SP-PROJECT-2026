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

    int main()
    {
        // trying to make a textbox
        string input;
        RectangleShape Tbox1(Vector2f(300, 40));
        Tbox1.setPosition(150, 150);
        Tbox1.setFillColor(Color::White);
        Tbox1.setOutlineColor(Color::Black);
        Tbox1.setOutlineThickness(2);
        unsigned int height = 800;
        unsigned int width = 1600;
        RenderWindow window(VideoMode(width, height), "SFML works!");

        window.setFramerateLimit(60);
        window.setKeyRepeatEnabled(false);

        Font font;
        if (!font.loadFromFile("Fonts/Blockletter.otf")) {
            cerr << "ERROR :: COULD_NOT_OPEN_FROM_FILE :: MAIN::Fonts/Blockletter.otf" << endl;
        }
        Text inputbox;
        inputbox.setFont(font);
        inputbox.setFillColor(Color::Black);
        inputbox.setString("Please Type Your Full Name");
        inputbox.setCharacterSize(24);
        inputbox.setPosition(149, 119);


        Text text;
        text.setFont(font);
        text.setString("Menawareen ya5watyyy\nTeam Members:\nSamir\nSofia\nelIona\nMona\nEbram\nSteevn\nMarwan");
        text.setCharacterSize(60);
        FloatRect tb = text.getGlobalBounds();
        text.setOrigin(tb.width * 0.5f, tb.height * 0.5f);
        text.setPosition(width / 2.f, height / 2.f);

        RectButton button(sf::Vector2f(150.f, 50.f), sf::Vector2f(325.f, 275.f));
        button.setButtonLabel(24, "Click Me!");
        button.setButtonFont(font);
        button.setLabelColor(Color::Red);
        button.setButtonColor(Color::Blue);


        RectangleShape topBar(Vector2f(1600, 40));
        topBar.setPosition(0, 0);
        topBar.setFillColor(Color(31, 11, 64));

        //// Menu texts
        //const int num_of_txt = 3;
        //vector<Text> menu_txt;
        //string menu_str[num_of_txt] = { "Admin Log In", "Employee Log In", "Exit" };
        //for (int i = 0; i < num_of_txt; i++) {
        //    Text t;
        //    t.setFont(font);
        //    t.setString(menu_str[i]);
        //    t.setCharacterSize(60);
        //    t.setPosition(500.f, 100.f * i + 50.f);
        //    menu_txt.push_back(t);
        //}
        //if (!menu_txt.empty())
        //    menu_txt[0].setFillColor(Color::Red);

        //int counter = 0;

        RectangleShape adminButton(Vector2f(400.f, 200.f));
        adminButton.setOrigin(400.f * 0.5f, 200.f * 0.5f);
        adminButton.setPosition(width / 2.f, height / 2.f);
        adminButton.setFillColor(Color::Black);

        while (window.isOpen())
        {
            Event event;
            if (event.type == Event::TextEntered) {
                if (event.text.unicode == '\b') {
                    if (!input.empty())
                        input.pop_back();
                    else if (event.text.unicode < 128) {
                        input += static_cast<char>(event.text.unicode);
                    }
                }
            }
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                    window.close();
                button.getButtonStatus(window, event);
                if (button.isPressed)
                {
                    cout << "Button was pressed!" << endl;
                    button.setButtonColor(Color::Green);
                }
                else if (button.isHover)
                    button.setButtonColor(Color::Yellow);
                else
                    button.setButtonColor(Color::Blue);

            }

            // Update
            Vector2f mouse_position = Vector2f(Mouse::getPosition(window));
            if (adminButton.getGlobalBounds().contains(mouse_position))
            {
                if (Mouse::isButtonPressed(Mouse::Left))
                    adminButton.setFillColor(Color::Yellow);
                else
                    adminButton.setFillColor(Color::Blue);
            }
            else
                adminButton.setFillColor(Color::Black);


            // Draw
            window.clear(Color::White);
            window.draw(topBar);
            button.draw(window);
            window.draw(inputbox);
            window.draw(Tbox1);
            window.display();
        }
    }

        return 0;
    }