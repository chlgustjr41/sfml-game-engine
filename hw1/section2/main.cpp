#include <SFML/Graphics.hpp>

#include "PlatformShape.h"
#include "CharacterShape.h"
#include "MovingPlatformShape.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const int PLATFORM_HEIGHT = 50;

const int CHARACTER_SIZE = 75;

const int MOVING_PLATFORM_WIDTH = 100;

const int MOVING_PLATFORM_MOVEMENT = 5;

int main()
{
    // create the window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "My window");
    window.setFramerateLimit(60);

    PlatformShape platform(sf::Vector2f(window.getSize().x, PLATFORM_HEIGHT), sf::Vector2f(0, window.getSize().y - PLATFORM_HEIGHT));

    CharacterShape character(CHARACTER_SIZE, sf::Vector2f(window.getSize().x / 2, platform.getPosition().y - CHARACTER_SIZE));
    sf::Texture texture;
    texture.loadFromFile("ncsu_logo.png");
    character.setTexture(&texture);

    MovingPlatformShape movingPlatform(sf::Vector2f(MOVING_PLATFORM_WIDTH, PLATFORM_HEIGHT), sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2));
    bool directionFlag = true;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);

        window.draw(platform);
        
        window.draw(character);

        // Moving platform repeatively moving left and right
        if (directionFlag && movingPlatform.getPosition().x + MOVING_PLATFORM_WIDTH > window.getSize().x)
            directionFlag = false;
        else if (directionFlag)
            movingPlatform.setPosition(movingPlatform.getPosition().x + MOVING_PLATFORM_MOVEMENT, movingPlatform.getPosition().y);
        else if (!directionFlag && movingPlatform.getPosition().x < 0)
            directionFlag = true;
        else if (!directionFlag)
            movingPlatform.setPosition(movingPlatform.getPosition().x - MOVING_PLATFORM_MOVEMENT, movingPlatform.getPosition().y);
        window.draw(movingPlatform);

        window.display();
    }

    return 0;
}