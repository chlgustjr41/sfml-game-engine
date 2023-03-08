#include "MovableObject.h"

MovableObject::MovableObject(EventManager *initialEventManager,
                             std::string objectType,
                             int objectIndex,
                             sf::RectangleShape *initialMovingObject)
{
    eventManager = initialEventManager;
    duration = 0;
    type = objectType;
    index = objectIndex;
    movingObject = initialMovingObject;
}

EventManager *MovableObject::getEventManager()
{
    return eventManager;
}
std::string MovableObject::getType()
{
    return type;
}
int MovableObject::getIndex()
{
    return index;
}
sf::RectangleShape *MovableObject::getMovingObject()
{
    return movingObject;
}
double MovableObject::getDuration()
{
    return duration;
}

void MovableObject::setEventManager(EventManager *newEventManager)
{
    eventManager = newEventManager;
}
void MovableObject::setType(std::string newType)
{
    type = newType;
}
void MovableObject::setIndex(int newIndex)
{
    index = newIndex;
}
void MovableObject::setMovingObject(sf::RectangleShape *newMovingObject)
{
    movingObject = newMovingObject;
}
void MovableObject::setDuration(double newDuration)
{
    duration = newDuration;
}

std::string MovableObject::getHeader()
{
    return getType() + std::to_string(getIndex());
}