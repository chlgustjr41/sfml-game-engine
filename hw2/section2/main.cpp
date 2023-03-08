#include <SFML/Graphics.hpp>
#include <iostream>
#include "Timeline.h"

const int WIDTH = 800;
const int HEIGHT = 600;

const float ROTATION_SPEED = 1;
const float TIME_RATIO = 1;

const float OBJECT_SIZE = 100;

const double DEFAULT_TICK = 8;

int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Timeline Test");
    window.setFramerateLimit(60);

    sf::RectangleShape object(sf::Vector2f(OBJECT_SIZE, OBJECT_SIZE));
    object.setOrigin(OBJECT_SIZE / 2, OBJECT_SIZE / 2);
    object.setPosition(window.getSize().x / 2, window.getSize().y / 2);

    Timeline timeline(DEFAULT_TICK);

    while (window.isOpen())
    {
        double duration = timeline.getDuration();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                if (timeline.getTick() < DEFAULT_TICK + DEFAULT_TICK / 2)
                {
                    timeline.increaseTick(DEFAULT_TICK / 2);
                    std::cout << "Current Tick: " << timeline.getTick() << std::endl;
                }
                else
                {
                    std::cout << "Tick cannot go above " << timeline.getTick() << std::endl;
                }
                if (timeline.getTick() == DEFAULT_TICK)
                {
                    std::cout << "Time Ratio: 1" << std::endl;
                }
                else if (timeline.getTick() > DEFAULT_TICK)
                {
                    std::cout << "Time Ratio: 0.5" << std::endl;
                }
                else if (timeline.getTick() < DEFAULT_TICK)
                {
                    std::cout << "Time Ratio: 1.5" << std::endl;
                }
            }
            else if (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                if (timeline.getTick() > DEFAULT_TICK / 2)
                {
                    timeline.decreaseTick(DEFAULT_TICK / 2);
                    std::cout << "Current Tick: " << timeline.getTick() << std::endl;
                }
                else
                {
                    std::cout << "Tick cannot go below " << timeline.getTick() << std::endl;
                }
                if (timeline.getTick() == DEFAULT_TICK)
                {
                    std::cout << "Time Ratio: 1" << std::endl;
                }
                else if (timeline.getTick() > DEFAULT_TICK)
                {
                    std::cout << "Time Ratio: 0.5" << std::endl;
                }
                else if (timeline.getTick() < DEFAULT_TICK)
                {
                    std::cout << "Time Ratio: 1.5" << std::endl;
                }
            }
            else if (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                if (timeline.getIsPaused())
                {
                    timeline.resume();
                    std::cout << "Current Tick: " << timeline.getTick() << std::endl;
                    std::cout << "Resume" << std::endl;
                }
                else
                {
                    timeline.pause();
                    std::cout << "Current Tick: " << timeline.getTick() << std::endl;
                    std::cout << "Paused" << std::endl;
                }
            }
        }

        object.rotate(ROTATION_SPEED * TIME_RATIO * duration);

        window.clear(sf::Color::Black);
        window.draw(object);
        window.display();
    }

    return 0;
}