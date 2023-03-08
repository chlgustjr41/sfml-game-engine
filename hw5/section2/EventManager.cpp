#include "EventManager.h"

const int EventManager::CHARACTER_INPUT_PRIORITY = 2;
const int EventManager::CHARACTER_DEATH_PRIORITY = 4;
const int EventManager::CHARACTER_SPAWN_PRIORITY = 3;
const int EventManager::PROJECTILE_CLEAR_PRIORITY = 1;
const int EventManager::PROJECTILE_LAUNCH_PRIORITY = 1;

EventManager::EventManager() {}

EventObject *EventManager::pollEvent(std::string eventType, std::string header)
{
    if (events.size() > 0 && eventType == events.back()->getEventType())
    {
        if (header == events.back()->getSoruceHeader())
        {
            EventObject *event = events.back();
            events.pop_back();
            return event;
        }
        else
        {
            return NULL;
        }
    }

    return NULL;
}
void EventManager::registerEvent(EventObject *newEvent)
{
    if (events.size() == 0)
    {
        events.push_back(newEvent);
    }
    else
    {
        for (int i = 0; i < events.size(); i++)
        {
            if (newEvent->getPriority() < events[i]->getPriority())
            {
                events.insert(events.begin() + i, newEvent);
                break;
            }
        }
    }
}