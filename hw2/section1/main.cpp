#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <thread>
#include <mutex>

#include "CharacterThread.h"
#include "PlatformThread.h"
#include "MovingPlatformThread.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const int PLATFORM_HEIGHT = 50;

const int CHARACTER_SIZE = 75;

const int MOVING_PLATFORM_WIDTH = 100;

const int MOVING_PLATFORM_MOVEMENT = 5;

const int PLATFORM_COUNT = 2;

const std::string CHARACTER_TYPE = "character";
const std::string PLATFORM_TYPE = "platform";
const std::string MOVING_PLATFORM_TYPE = "moving platform";

void handleThreadObjectRun(ThreadObject *obj)
{
    obj->run();
}

int main()
{
    // std::mutex mutex;
    // std::condition_variable condition;

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "My Game");
    window.setFramerateLimit(60);

    PlatformThread platform(sf::Vector2f(window.getSize().x, window.getSize().y),
                            sf::Vector2f(window.getSize().x, PLATFORM_HEIGHT),
                            sf::Vector2f(0, window.getSize().y - PLATFORM_HEIGHT));

    MovingPlatformThread movingPlatform(sf::Vector2f(window.getSize().x, window.getSize().y),
                                        sf::Vector2f(MOVING_PLATFORM_WIDTH, PLATFORM_HEIGHT),
                                        sf::Vector2f(window.getSize().x / 2 - MOVING_PLATFORM_WIDTH / 2, window.getSize().y / 2));

    ThreadObject *platforms[PLATFORM_COUNT];
    platforms[0] = &platform;
    platforms[1] = &movingPlatform;

    CharacterThread character(platforms,
                              PLATFORM_COUNT,
                              sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT),
                              sf::Vector2f(CHARACTER_SIZE, CHARACTER_SIZE),
                              sf::Vector2f(WINDOW_WIDTH / 2 - CHARACTER_SIZE / 2, platform.getPosition().y - CHARACTER_SIZE + 0.5));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                character.setIsMovingLeft(true);
            else
                character.setIsMovingLeft(false);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                character.setIsMovingRight(true);
            else
                character.setIsMovingRight(false);

            if (!character.getIsJumping() && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                character.setIsJumping(true);
                character.move(0, 0.5); // 0.5 jump to get off of intersect state
            }
        }

        std::thread characterThread(handleThreadObjectRun, &character);
        std::thread movingPlatformThread(handleThreadObjectRun, &movingPlatform);

        characterThread.join();
        movingPlatformThread.join();

        window.clear(sf::Color::Black);
        for (int i = 0; i < PLATFORM_COUNT; i++)
            window.draw((*platforms[i]));
        window.draw(character);
        window.display();
    }

    return 0;
}