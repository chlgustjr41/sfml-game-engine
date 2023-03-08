#include "EventManager.h"

const int EventManager::CHARACTER_INPUT_PRIORITY = 2;
const int EventManager::CHARACTER_DEATH_PRIORITY = 4;
const int EventManager::CHARACTER_SPAWN_PRIORITY = 3;
const int EventManager::CHARACTER_PLATFORM_COLLIDE_PRIORITY = 1;

EventManager::EventManager() {}

EventObject *EventManager::pollEvent(std::string eventType, std::string header)
{
    if (events.size() > 0)
    {
        if (eventType == events.back()->getEventType() &&
            header == events.back()->getSoruceHeader())
        {
            EventObject *event = events.back();
            events.pop_back();
            // std::cout << "Event Polled: " << header << "-" << eventType << "-" << event->getContent() << std::endl;
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
    // std::cout << "Event Registered: " << newEvent->getSoruceHeader() << "-" << newEvent->getEventType() << "-" << newEvent->getContent() << std::endl;
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