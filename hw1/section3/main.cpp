#include <SFML/Graphics.hpp>

#include <math.h>
#include <iostream>

#include "PlatformShape.h"
#include "CharacterShape.h"
#include "MovingPlatformShape.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const int PLATFORM_HEIGHT = 50;

const int CHARACTER_SIZE = 75;

const int MOVING_PLATFORM_WIDTH = 100;

const int MOVING_PLATFORM_MOVEMENT = 5;

const int CHARACTER_MOVEMENT = 5;

const int JUMP_HEIGHT = 15;
const int JUMP_FRAME = 60;

int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "My window");
    window.setFramerateLimit(60);

    // Platform
    PlatformShape platform(sf::Vector2f(window.getSize().x, PLATFORM_HEIGHT), sf::Vector2f(0, window.getSize().y - PLATFORM_HEIGHT));

    // Character
    CharacterShape character(CHARACTER_SIZE, sf::Vector2f(window.getSize().x / 2, platform.getPosition().y - CHARACTER_SIZE));
    sf::Texture texture;
    texture.loadFromFile("ncsu_logo.png");
    character.setTexture(&texture);
    
    bool isMovingLeft = false;
    bool isMovingRight = false;

    bool isJumping = false;
    int jumpTime = 0;

    // Moving Platform
    MovingPlatformShape movingPlatform(sf::Vector2f(MOVING_PLATFORM_WIDTH, PLATFORM_HEIGHT), sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2));
    
    bool directionFlag = true;

    // Drawing
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                isMovingLeft = true;
            else
                isMovingLeft = false;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                isMovingRight = true;
            else
                isMovingRight = false;

            if (!isJumping && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                isJumping = true;
                jumpTime = 0;
            }
        }

        window.clear(sf::Color::Black);

        window.draw(platform);
        
        // Character left movement
        if (isMovingLeft && character.getPosition().x >= 0) 
            character.setPosition(character.getPosition().x - CHARACTER_MOVEMENT, character.getPosition().y);
        // Character left movement
        if (isMovingRight && character.getPosition().x + CHARACTER_SIZE <= window.getSize().x) 
            character.setPosition(character.getPosition().x + CHARACTER_MOVEMENT, character.getPosition().y);
        // Character jumping movement
        if (isJumping)
        {
            // Jump logic for smooth jumping with different acceleration
            character.move(0, -JUMP_HEIGHT * std::cos(M_PI * jumpTime / JUMP_FRAME));
            if (jumpTime < JUMP_FRAME)
                jumpTime += 1;
            else {
                isJumping = false;
                jumpTime = 0;
            }
        }
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