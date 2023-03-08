#include <SFML/Graphics.hpp>
#include <string>
#include <chrono>
#include <thread>
#include <iostream>
#include <mutex>
#include "CharacterObject.h"
#include "Timeline.h"
#include "StaticPlatformObject.h"
#include "VerticallyMovingPlatformObject.h"
#include "HorizontallyMovingPlatformObject.h"
#include "DeathZoneObject.h"
#include "ViewObject.h"

int main(int argc, char *argv[])
{
    const float WINDOW_WIDTH = 800.0;
    const float WINDOW_HEIGHT = 600.0;
    const float CHARACTER_SIZE = 75.0;
    const float PLATFORM_HEIGHT = 50.0;
    const float MOVING_PLATFORM_WIDTH = 100.0;

    const long TICK_SIZE = 1;
    const std::vector<sf::Color> CHARACTER_COLORS = {sf::Color::Red, sf::Color::Blue, sf::Color::Green, sf::Color::Yellow, sf::Color::Magenta};

    sf::RenderWindow *window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Game");
    window->setFramerateLimit(60);

    Timeline *timeline = new Timeline(TICK_SIZE);

    // Static platform 1
    sf::RectangleShape *staticPlatformShape1 = new sf::RectangleShape(sf::Vector2f(WINDOW_WIDTH, PLATFORM_HEIGHT));
    staticPlatformShape1->setPosition(0, WINDOW_HEIGHT - PLATFORM_HEIGHT);
    StaticPlatformObject *staticPlatform1 = new StaticPlatformObject(staticPlatformShape1);
    // Static platform 2
    sf::RectangleShape *staticPlatformShape2 = new sf::RectangleShape(sf::Vector2f(WINDOW_WIDTH, PLATFORM_HEIGHT));
    staticPlatformShape2->setPosition(WINDOW_WIDTH + CHARACTER_SIZE + 100, WINDOW_HEIGHT - PLATFORM_HEIGHT);
    StaticPlatformObject *staticPlatform2 = new StaticPlatformObject(staticPlatformShape2);
    // Vertically moving platform 1
    sf::RectangleShape *verticallyMovingPlatformShape1 = new sf::RectangleShape(sf::Vector2f(MOVING_PLATFORM_WIDTH, PLATFORM_HEIGHT));
    verticallyMovingPlatformShape1->setPosition(300, WINDOW_HEIGHT / 2);
    VerticallyMovingPlatformObject *verticallyMovingPlatform1 = new VerticallyMovingPlatformObject(verticallyMovingPlatformShape1, 400);
    // Vertically moving platform 2
    sf::RectangleShape *verticallyMovingPlatformShape2 = new sf::RectangleShape(sf::Vector2f(MOVING_PLATFORM_WIDTH, PLATFORM_HEIGHT));
    verticallyMovingPlatformShape2->setPosition(1100, WINDOW_HEIGHT / 2);
    VerticallyMovingPlatformObject *verticallyMovingPlatform2 = new VerticallyMovingPlatformObject(verticallyMovingPlatformShape2, 400);
    // Horizontally moving platform 1
    sf::RectangleShape *horizontallyMovingPlatformShape1 = new sf::RectangleShape(sf::Vector2f(MOVING_PLATFORM_WIDTH, PLATFORM_HEIGHT));
    horizontallyMovingPlatformShape1->setPosition(850, 100);
    HorizontallyMovingPlatformObject *horizontallyMovingPlatform1 = new HorizontallyMovingPlatformObject(horizontallyMovingPlatformShape1, 300);
    // Platforms
    std::vector<GameObject *> platforms;
    platforms.push_back(staticPlatform1);
    platforms.push_back(staticPlatform2);
    platforms.push_back(verticallyMovingPlatform1);
    platforms.push_back(verticallyMovingPlatform2);
    platforms.push_back(horizontallyMovingPlatform1);

    // Character
    sf::RectangleShape *characterBoundary = new sf::RectangleShape(sf::Vector2f(WINDOW_WIDTH * 4, WINDOW_HEIGHT));
    characterBoundary->setPosition(-WINDOW_WIDTH, 0);
    sf::RectangleShape *characterShape = new sf::RectangleShape(sf::Vector2f(CHARACTER_SIZE, CHARACTER_SIZE));
    characterShape->setPosition(100, (WINDOW_HEIGHT - CHARACTER_SIZE) / 2);
    CharacterObject *character = new CharacterObject(characterShape, characterBoundary, &platforms);
    // Characters
    std::vector<CharacterObject *> characters;
    characters.push_back(character);

    // View adjuster for the character
    sf::RectangleShape *viewBoundary = new sf::RectangleShape(sf::Vector2f(CHARACTER_SIZE * 2, CHARACTER_SIZE * 2));
    viewBoundary->setPosition(characterShape->getPosition().x - (viewBoundary->getSize().x - characterShape->getSize().x) / 2,
                              characterShape->getPosition().y - (viewBoundary->getSize().y - characterShape->getSize().y) / 2);
    viewBoundary->setFillColor(sf::Color::Blue);
    ViewObject *view = new ViewObject(viewBoundary, characterShape);
    // Set default starting view
    sf::View windowView = window->getView();
    windowView.setCenter(characterShape->getPosition().x, windowView.getCenter().y);
    window->setView(windowView);

    // Spawn/Death Zone
    sf::RectangleShape *spawnZoneShape = new sf::RectangleShape(sf::Vector2f(CHARACTER_SIZE, CHARACTER_SIZE));
    spawnZoneShape->setPosition(100, WINDOW_HEIGHT / 2 - 200);
    spawnZoneShape->setFillColor(sf::Color::Green);
    sf::RectangleShape *deathZoneShape = new sf::RectangleShape(sf::Vector2f(WINDOW_WIDTH * 5, PLATFORM_HEIGHT));
    deathZoneShape->setPosition(-WINDOW_WIDTH, WINDOW_HEIGHT + PLATFORM_HEIGHT);
    deathZoneShape->setFillColor(sf::Color::Red);
    DeathZoneObject *deathZone = new DeathZoneObject(deathZoneShape, spawnZoneShape, &characters);

    bool isRunning = true;
    while (isRunning)
    {
        double duration = timeline->getDuration();
        character->updateDuration(duration);
        view->updateView(window);
        verticallyMovingPlatform1->updateDuration(duration);
        verticallyMovingPlatform2->updateDuration(duration);
        horizontallyMovingPlatform1->updateDuration(duration);

        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window->close();
                isRunning = false;
            }
        }

        // Character input
        std::string inputs = "";
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            inputs += "Left,";
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            inputs += "Right,";
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) &&
            !character->getIsJumping())
        {
            inputs += "Space,";
        }
        character->receiveInput(inputs);

        window->clear(sf::Color::Black);
        deathZone->checkDeathZone();

        // deathZone->render(window);
        // view->render(window);

        character->render(window);
        staticPlatform1->render(window);
        staticPlatform2->render(window);
        verticallyMovingPlatform1->render(window);
        verticallyMovingPlatform2->render(window);
        horizontallyMovingPlatform1->render(window);

        window->display();
    }
}