#include "EventObject.h"

const std::string EventObject::CHARACTER_INPUT = "CharacterInput";
const std::string EventObject::CHARACTER_DEATH = "CharacterDeath";
const std::string EventObject::CHARACTER_SPAWN = "CharacterSpawn";
const std::string EventObject::PROJECTILE_CLEAR = "ProjectileClear";
const std::string EventObject::PROJECTILE_LAUNCH = "ProjectileLaunch";

EventObject::EventObject(std::string raisedEventType,
                         int eventPriority,
                         std::string sourceObjectHeader,
                         std::string eventContent)
{
    eventType = raisedEventType;
    priority = eventPriority;
    sourceHeader = sourceObjectHeader;
    content = eventContent;
}

std::string EventObject::getEventType()
{
    return eventType;
}
int EventObject::getPriority()
{
    return priority;
}
std::string EventObject::getSoruceHeader()
{
    return sourceHeader;
}
std::string EventObject::getContent()
{
    return content;
}