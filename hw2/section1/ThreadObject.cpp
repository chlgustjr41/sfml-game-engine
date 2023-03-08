#include <SFML/Graphics.hpp>
#include <iostream>
#include "ThreadObject.h"

const std::string ThreadObject::CHARACTER_TYPE = "character";
const std::string ThreadObject::PLATFORM_TYPE = "platform";
const std::string ThreadObject::MOVING_PLATFORM_TYPE = "moving platform";
const std::string ThreadObject::UNKNOWN_TYPE = "unknown";

ThreadObject::ThreadObject(std::string name, 
                            const sf::Vector2f &wSize,
                            const sf::Vector2f &size, 
                            const sf::Vector2f &position): windowSize(wSize), m_size(size), threadName(name)
{
    setSize(m_size);
    setPosition(position.x, position.y);
    update();
}

std::string ThreadObject::getThreadName()
{
    return threadName;
}
sf::Vector2f ThreadObject::getWindowSize()
{
    return windowSize;
}
void ThreadObject::setWindowSize(sf::Vector2f size)
{
    windowSize = size;
}