#include <SFML/Graphics.hpp>
#include <iostream>

const int WIDTH = 800;
const int HEIGHT = 600;

const float ROTATION_SPEED = 1.0;

const int TOGGLE_BUTTON_RADIUS = 25;
const int BUTTON_PADDING = 10;

const int RELATIVE_SIZE_RATIO = 5;

int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT),  "Default: " + std::to_string(WIDTH) + ", " + std::to_string(HEIGHT), sf::Style::Resize);
    window.setFramerateLimit(60);
    // Setting view to avoid squeezing/stretching
    window.setView(sf::View(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y)));

    sf::Vector2f constantSize = sf::Vector2f(window.getSize().x / RELATIVE_SIZE_RATIO, window.getSize().y / RELATIVE_SIZE_RATIO);

    sf::RectangleShape object(constantSize);
    object.setOrigin(constantSize.x / 2, constantSize.y / 2);
    object.setPosition(window.getSize().x / 2, window.getSize().y / 2);

    bool toggleSizeMode = true; // false for Constant Size Mode, true for Proportional Size Mode
    float relativeSize = -1;

    sf::CircleShape toggleButton(TOGGLE_BUTTON_RADIUS);
    toggleButton.setPosition(window.getSize().x - TOGGLE_BUTTON_RADIUS * 2 - BUTTON_PADDING, BUTTON_PADDING);
    toggleButton.setFillColor(sf::Color::Blue);  // red for Constant Size Mode, blue for Relative Size Mode

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::Resized) 
            {
                // Resetting the position of each objects
                object.setPosition(window.getSize().x / 2, window.getSize().y / 2);
                toggleButton.setPosition(window.getSize().x - TOGGLE_BUTTON_RADIUS * 2 - BUTTON_PADDING, BUTTON_PADDING);

                // Resetting the view to avoid squeezing/stretching
                sf::FloatRect visibleArea(0, 0, window.getSize().x, window.getSize().y);
                window.setView(sf::View(visibleArea));
            }
            // Occurs when the mouse clicks the colored button
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::IntRect mouse(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), sf::Vector2i(1, 1));
                sf::IntRect relativeButton(window.mapCoordsToPixel(sf::Vector2f(toggleButton.getPosition().x, toggleButton.getPosition().y)), 
                                            window.mapCoordsToPixel(sf::Vector2f(TOGGLE_BUTTON_RADIUS * 2, TOGGLE_BUTTON_RADIUS * 2)));

                if (relativeButton.intersects(mouse))
                {
                    toggleSizeMode = !toggleSizeMode;
                    if (toggleSizeMode)
                        toggleButton.setFillColor(sf::Color::Blue);
                    else 
                        toggleButton.setFillColor(sf::Color::Red);
                }
             }
            // Occurs when any kind of key press is done
            if (event.type == sf::Event::KeyPressed)
            {
                toggleSizeMode = !toggleSizeMode;
                if (toggleSizeMode)
                    toggleButton.setFillColor(sf::Color::Blue);
                else 
                    toggleButton.setFillColor(sf::Color::Red);
            }
        }

        window.clear(sf::Color::Black);

        if (toggleSizeMode) 
        {
            // Always updating the size during Relative Size Mode
            constantSize = sf::Vector2f(window.getSize().x / RELATIVE_SIZE_RATIO, window.getSize().y / RELATIVE_SIZE_RATIO);
            object.setSize(constantSize);
            object.setOrigin(constantSize.x / 2, constantSize.y / 2);
        }
        else 
        {
            // Remembers the constant screen value that was recorded for Constant Size Mode
            object.setSize(constantSize);
            object.setOrigin(object.getSize().x / 2, object.getSize().y / 2);
        }

        // Constantly rotation object for aesthetic
        object.setRotation(object.getRotation() + ROTATION_SPEED);
        window.draw(object);

        window.draw(toggleButton);

        window.display();
    }

    return 0;
}