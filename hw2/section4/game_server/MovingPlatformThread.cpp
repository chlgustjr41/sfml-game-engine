#include "MovingPlatformThread.h"
#include <iostream>

MovingPlatformThread::MovingPlatformThread(const sf::Vector2f &wSize,
                                           const sf::Vector2f &size,
                                           const sf::Vector2f &position) : ThreadObject(MOVING_PLATFORM_TYPE, wSize, size, position)
{
    directionFlag = false;
}

bool MovingPlatformThread::getDirectionFlag()
{
    return directionFlag;
}

void MovingPlatformThread::run()
{
    if (directionFlag && (*this).getPosition().x + (*this).getSize().x > getWindowSize().x)
        directionFlag = false;
    else if (!directionFlag && (*this).getPosition().x < 0)
        directionFlag = true;

    if (directionFlag)
        (*this).setPosition((*this).getPosition().x + MOVING_PLATFORM_MOVEMENT * getDuration() * TIME_RATIO, (*this).getPosition().y);
    else
        (*this).setPosition((*this).getPosition().x - MOVING_PLATFORM_MOVEMENT * getDuration() * TIME_RATIO, (*this).getPosition().y);
}