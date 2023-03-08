#include "ThreadObject.h"

#ifndef PLATFORMTHREAD_H 
#define PLATFORMTHREAD_H

class PlatformThread: public ThreadObject
{
    public:
        explicit PlatformThread(const sf::Vector2f &wSize = sf::Vector2f(0.f, 0.f),
                                const sf::Vector2f &size = sf::Vector2f(0.f, 0.f), 
                                const sf::Vector2f &position = sf::Vector2f(0.f, 0.f));
        void run();
};

#endif