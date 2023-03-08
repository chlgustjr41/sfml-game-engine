#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include "EventObject.h"

class EventManager
{
public:
    static const int CHARACTER_INPUT_PRIORITY;
    static const int CHARACTER_DEATH_PRIORITY;
    static const int CHARACTER_SPAWN_PRIORITY;
    static const int CHARACTER_PLATFORM_COLLIDE_PRIORITY;

    explicit EventManager();

    EventObject *pollEvent(std::string eventType, std::string header);
    void registerEvent(EventObject *newEvent);

private:
    std::vector<EventObject *> events;
};

#endif