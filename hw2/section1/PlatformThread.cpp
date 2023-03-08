#include "PlatformThread.h"
#include <iostream>

PlatformThread::PlatformThread(const sf::Vector2f &wSize,
                                const sf::Vector2f &size, 
                                const sf::Vector2f &position): ThreadObject(PLATFORM_TYPE, wSize, size, position)
{
}

void PlatformThread::run()
{
    // Platform has no job to perform
}