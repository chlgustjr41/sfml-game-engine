#include <iostream>
#include "GameObject.h"
#include "MovableObject.h"
#include "VerticallyMovingPlatformMovableObject.h"

const double VerticallyMovingPlatformMovableObject::MOVING_PLATFORM_MOVEMENT = 0.5;

VerticallyMovingPlatformMovableObject::VerticallyMovingPlatformMovableObject(std::string objectType,
                                                                             sf::RectangleShape *movableobject,
                                                                             double initialMovementLength)
    : MovableObject(objectType, movableobject)
{
    directionFlag = false;
    originalXPosition = movableobject->getPosition().x;
    movementLength = initialMovementLength;
}

void VerticallyMovingPlatformMovableObject::attach(sf::RectangleShape *object)
{
    for (int i = 0; i < attachedObjects.size(); i++)
    {
        if (object == attachedObjects[i])
            return;
    }
    attachedObjects.push_back(object);
}
bool VerticallyMovingPlatformMovableObject::updateAttach(sf::RectangleShape *object)
{
    for (int i = 0; i < attachedObjects.size(); i++)
    {
        if (object == attachedObjects[i] && !MovableObject::getMovingObject()->getGlobalBounds().intersects(object->getGlobalBounds()))
        {
            attachedObjects.erase(attachedObjects.begin() + i);
            return false;
        }
    }

    return true;
}

bool VerticallyMovingPlatformMovableObject::getDirectionFlag()
{
    return directionFlag;
}
double VerticallyMovingPlatformMovableObject::getOriginalXPosition()
{
    return originalXPosition;
}
double VerticallyMovingPlatformMovableObject::getMovementLength()
{
    return movementLength;
}

void VerticallyMovingPlatformMovableObject::updateOriginalXPosition()
{
    originalXPosition = MovableObject::getMovingObject()->getPosition().x;
}
void VerticallyMovingPlatformMovableObject::setMovementLength(double newMovementLength)
{
    movementLength = newMovementLength;
}

void VerticallyMovingPlatformMovableObject::run()
{
    sf::RectangleShape *thisObject = MovableObject::getMovingObject();
    double duration = MovableObject::getDuration();

    double changeX = 0;
    double characterOffX = 0;

    if (directionFlag)
    {
        changeX += MOVING_PLATFORM_MOVEMENT * duration * GameObject::TIME_RATIO;
    }
    else
    {
        changeX -= MOVING_PLATFORM_MOVEMENT * duration * GameObject::TIME_RATIO;
    }

    if (directionFlag && thisObject->getPosition().x + thisObject->getSize().x > originalXPosition + movementLength)
    {
        directionFlag = false;
        characterOffX = -1 * changeX;
    }
    else if (!directionFlag && thisObject->getPosition().x < originalXPosition)
    {
        directionFlag = true;
        characterOffX = -1 * changeX;
    }

    thisObject->move(changeX, 0);
    for (int i = 0; i < attachedObjects.size(); i++)
    {
        sf::RectangleShape *attachedObject = attachedObjects[i];

        if (updateAttach(attachedObject) && changeX != 0)
            attachedObject->move(changeX + characterOffX, 0);
    }
}