#include <iostream>
#include "GameObject.h"
#include "MovableObject.h"
#include "HorizontallyMovingPlatformMovableObject.h"

const double HorizontallyMovingPlatformMovableObject::MOVING_PLATFORM_MOVEMENT = 0.5;

HorizontallyMovingPlatformMovableObject::HorizontallyMovingPlatformMovableObject(std::string objectType,
                                                                                 sf::RectangleShape *movableobject,
                                                                                 double initialMovementLength)
    : MovableObject(objectType, movableobject)
{
    directionFlag = false;
    originalYPosition = movableobject->getPosition().y;
    movementLength = initialMovementLength;
}

void HorizontallyMovingPlatformMovableObject::attach(sf::RectangleShape *object)
{
    for (int i = 0; i < attachedObjects.size(); i++)
    {
        if (object == attachedObjects[i])
            return;
    }
    attachedObjects.push_back(object);
}
bool HorizontallyMovingPlatformMovableObject::updateAttach(sf::RectangleShape *object)
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

bool HorizontallyMovingPlatformMovableObject::getDirectionFlag()
{
    return directionFlag;
}
double HorizontallyMovingPlatformMovableObject::getOriginalYPosition()
{
    return originalYPosition;
}
double HorizontallyMovingPlatformMovableObject::getMovementLength()
{
    return movementLength;
}

void HorizontallyMovingPlatformMovableObject::updateOriginalYPosition()
{
    originalYPosition = MovableObject::getMovingObject()->getPosition().x;
}
void HorizontallyMovingPlatformMovableObject::setMovementLength(double newMovementLength)
{
    movementLength = newMovementLength;
}

void HorizontallyMovingPlatformMovableObject::run()
{
    sf::RectangleShape *thisObject = MovableObject::getMovingObject();
    double duration = MovableObject::getDuration();

    double changeY = 0;
    double characterOffY = 0;

    if (directionFlag)
    {
        changeY += MOVING_PLATFORM_MOVEMENT * duration * GameObject::TIME_RATIO;
    }
    else
    {
        changeY -= MOVING_PLATFORM_MOVEMENT * duration * GameObject::TIME_RATIO;
    }

    if (directionFlag && thisObject->getPosition().y + thisObject->getSize().y > originalYPosition + movementLength)
    {
        directionFlag = false;
        characterOffY = -1 * changeY;
    }
    else if (!directionFlag && thisObject->getPosition().y < originalYPosition)
    {
        directionFlag = true;
        characterOffY = -1 * changeY;
    }

    thisObject->move(0, changeY);
    for (int i = 0; i < attachedObjects.size(); i++)
    {
        sf::RectangleShape *attachedObject = attachedObjects[i];

        if (updateAttach(attachedObject) && changeY != 0)
            attachedObject->move(0, changeY + characterOffY);
    }
}