#include <SFML/Graphics.hpp>
#include "MovableObject.h"

MovableObject::MovableObject(std::string objectType, sf::RectangleShape *initialMovingObject)
{
    duration = 0;
    movingObject = initialMovingObject;
}

std::string MovableObject::getType()
{
    return type;
}
sf::RectangleShape *MovableObject::getMovingObject()
{
    return movingObject;
}
double MovableObject::getDuration()
{
    return duration;
}

void MovableObject::setType(std::string newType)
{
    type = newType;
}
void MovableObject::setMovingObject(sf::RectangleShape *newMovingObject)
{
    movingObject = newMovingObject;
}
void MovableObject::setDuration(double newDuration)
{
    duration = newDuration;
}