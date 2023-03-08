#include <SFML/Graphics.hpp>

#include <cmath>
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

const int CHARACTER_MOVEMENT = 10;

const int JUMP_HEIGHT = 15;
const int JUMP_FRAME = 60;

int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "My window");
    window.setFramerateLimit(60);

    // Platform
    PlatformShape platform(sf::Vector2f(window.getSize().x, PLATFORM_HEIGHT), sf::Vector2f(0, window.getSize().y - PLATFORM_HEIGHT));

    // Character
    CharacterShape character(CHARACTER_SIZE, sf::Vector2f(window.getSize().x / 2, platform.getPosition().y - CHARACTER_SIZE + 1));
    sf::Texture texture;
    texture.loadFromFile("ncsu_logo.png");
    character.setTexture(&texture);

    bool isMovingLeft = false; // for user key press input indication
    bool isMovingRight = false; // for user key press input indication

    bool isJumping = false; // flag that indicates the jumping state of the character
    float jumpTime = 0; // represented as radius value to calculate smooth jumping animation

    // Moving Platform
    MovingPlatformShape movingPlatform(sf::Vector2f(MOVING_PLATFORM_WIDTH, PLATFORM_HEIGHT), sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2));
    
    bool directionFlag = true; // flag that indicates the direction of the moving platform

    bool isOnMovingPlatform = false; // flag that indicates if the character is on the moving platform

    // Drawing
    bool isRunning = true;
    while (isRunning)
    {
        if (window.hasFocus())
        {
            window.setActive(true);
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed) {
                    window.close();
                    isRunning = false;
                }

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

            // Check if the character is off the moving platform
            if (!character.getGlobalBounds().intersects(movingPlatform.getGlobalBounds())) 
                isOnMovingPlatform = false;
            
            // Character left movement
            if (isMovingLeft && character.getPosition().x >= 0) 
            {
                character.setPosition(character.getPosition().x - CHARACTER_MOVEMENT, character.getPosition().y);
            }
            // Character right movement
            if (isMovingRight && character.getPosition().x + CHARACTER_SIZE <= window.getSize().x) 
            {
                character.setPosition(character.getPosition().x + CHARACTER_MOVEMENT, character.getPosition().y);
            }
            // Character jumping movement
            if (isJumping)
            {
                // Jump logic for smooth jumping with different acceleration
                character.move(0, -JUMP_HEIGHT * std::cos(M_PI * jumpTime / JUMP_FRAME));
                if (jumpTime < JUMP_FRAME)
                    jumpTime += 1;

                // Landing on the ground
                if (character.getGlobalBounds().intersects(platform.getGlobalBounds()))
                {
                    isJumping = false;
                    jumpTime = 0;

                    character.setPosition(character.getPosition().x, platform.getPosition().y - CHARACTER_SIZE + 0.1); // 0.1 buffer to intersect
                }
                // Landing on moving platform
                else if (!isOnMovingPlatform && character.getGlobalBounds().intersects(movingPlatform.getGlobalBounds()) && 
                            character.getPosition().y < movingPlatform.getPosition().y)
                {
                    isOnMovingPlatform = true;

                    isJumping = false;
                    jumpTime = 0;

                    character.setPosition(character.getPosition().x, movingPlatform.getPosition().y - CHARACTER_SIZE + 0.1); // 0.1 buffer to intersect
                }
                // Hitting the moving platform (ceiling) while jumping
                else if (character.getGlobalBounds().intersects(movingPlatform.getGlobalBounds()) && 
                            character.getPosition().y > movingPlatform.getPosition().y)
                {
                    jumpTime = JUMP_FRAME / 2 + 5; // falling + bounce out
                }
            }
            // Free falling
            else if (!character.getGlobalBounds().intersects(platform.getGlobalBounds()) &&
                                    !character.getGlobalBounds().intersects(movingPlatform.getGlobalBounds()))
            {
                isJumping = true;
                jumpTime = JUMP_FRAME / 2;
            }
            // Character on moving platform movement
            else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
                character.getGlobalBounds().intersects(movingPlatform.getGlobalBounds()) &&
                character.getPosition().y <= movingPlatform.getPosition().y - PLATFORM_HEIGHT &&
                isOnMovingPlatform)
            {
                if (directionFlag)
                    character.setPosition(character.getPosition().x + MOVING_PLATFORM_MOVEMENT, character.getPosition().y); 
                else
                    character.setPosition(character.getPosition().x - MOVING_PLATFORM_MOVEMENT, character.getPosition().y);
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
        window.setActive(false);
    }

    return 0;
}