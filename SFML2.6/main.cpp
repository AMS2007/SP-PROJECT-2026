#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <vector>
using namespace sf;
using namespace std;

int main()
{
    unsigned int height = 800;
    unsigned int width = 1600;
    RenderWindow window(VideoMode(width, height), "SFML works!");

    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);

    sf::Font font;
    if (!font.loadFromFile("Fonts/Blockletter.otf")) {
        std::cerr << "ERROR :: COULD_NOT_OPEN_FROM_FILE :: MAIN::Fonts/Blockletter.otf" << std::endl;
    }

    std::cout << "Kerning : " << font.getKerning('A', 'B', 12) << std::endl;

    Text text;
    text.setFont(font);
    text.setString("Menawareen ya5watyyy\nTeam Members:\nSamir\nSofia\nelIona\nMona\nEbram\nSteevn\nMarwan");
    text.setCharacterSize(60);
    sf::FloatRect tb = text.getGlobalBounds();
    text.setOrigin(tb.width * 0.5f, tb.height * 0.5f);
    text.setPosition(width / 2.f, height / 2.f);

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
    adminButton.setFillColor(Color::White);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
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
            adminButton.setFillColor(Color::White);

        // Draw
        window.clear(Color::Black);
        window.draw(topBar);
        window.draw(adminButton);
        window.display();
    }

    return 0;
}