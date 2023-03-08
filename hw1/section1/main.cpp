#include <SFML/Graphics.hpp>

const int WIDTH = 800;
const int HEIGHT = 600;

int main()
{
    sf::Window window(sf::VideoMode(800, 600),  "Default: " + std::to_string(WIDTH) + ", " + std::to_string(HEIGHT), sf::Style::Resize);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::Resized && window.getSize().x != WIDTH && window.getSize().y != HEIGHT )
                window.setTitle("Resized: " + std::to_string(window.getSize().x) + ", " + std::to_string(window.getSize().y));
        }
    }

    return 0;
}