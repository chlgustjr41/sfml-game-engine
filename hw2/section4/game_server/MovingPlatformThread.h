#include "ThreadObject.h"

#ifndef MOVINGPLATFORMTHREAD_H
#define MOVINGPLATFORMTHREAD_H

class MovingPlatformThread : public ThreadObject
{
public:
    static const int MOVING_PLATFORM_MOVEMENT = 1;

    explicit MovingPlatformThread(const sf::Vector2f &wSize = sf::Vector2f(0.f, 0.f),
                                  const sf::Vector2f &size = sf::Vector2f(0.f, 0.f),
                                  const sf::Vector2f &position = sf::Vector2f(0.f, 0.f));

    bool getDirectionFlag();

    void run();

private:
    bool directionFlag;
};

#endif