#include <iostream>
#include "GameObject.h"
#include "MovableObject.h"
#include "ViewMovableObject.h"

ViewMovableObject::ViewMovableObject(std::string objectType,
                                     sf::RectangleShape *initialObjectShape,
                                     sf::RectangleShape *gameCharacter) : MovableObject(objectType, initialObjectShape)
{
    character = gameCharacter;
    isTrackingLeft = false;
    isTrackingRight = false;
    isTrackingDown = false;
    isTrackingUp = false;
}

sf::RectangleShape *ViewMovableObject::getCharacter()
{
    return character;
}

bool ViewMovableObject::getIsTrackingLeft()
{
    return isTrackingLeft;
}
bool ViewMovableObject::getIsTrackingRight()
{
    return isTrackingRight;
}
bool ViewMovableObject::getIsTrackingDown()
{
    return isTrackingDown;
}
bool ViewMovableObject::getIsTrackingUp()
{
    return isTrackingUp;
}

void ViewMovableObject::run()
{
    sf::RectangleShape *characterBoundary = MovableObject::getMovingObject();

    if (character->getPosition().x < characterBoundary->getPosition().x)
    {
        isTrackingLeft = true;
        isTrackingRight = false;
        isTrackingDown = false;
        isTrackingUp = false;

        characterBoundary->setPosition(character->getPosition().x, characterBoundary->getPosition().y);
    }
    else if (character->getPosition().x + character->getSize().x > characterBoundary->getPosition().x + characterBoundary->getSize().x)
    {
        isTrackingLeft = false;
        isTrackingRight = true;
        isTrackingDown = false;
        isTrackingUp = false;

        characterBoundary->setPosition(character->getPosition().x + character->getSize().x - characterBoundary->getSize().x, characterBoundary->getPosition().y);
    }
    else if (character->getPosition().y + character->getSize().y > characterBoundary->getPosition().y + characterBoundary->getSize().y)
    {
        isTrackingLeft = false;
        isTrackingRight = false;
        isTrackingDown = true;
        isTrackingUp = false;

        characterBoundary->setPosition(characterBoundary->getPosition().x, character->getPosition().y + character->getSize().y - characterBoundary->getSize().y);
    }
    else if (character->getPosition().y < characterBoundary->getPosition().y)
    {
        isTrackingLeft = false;
        isTrackingRight = false;
        isTrackingDown = false;
        isTrackingUp = true;

        characterBoundary->setPosition(characterBoundary->getPosition().x, character->getPosition().y);
    }
    else
    {
        isTrackingLeft = false;
        isTrackingRight = false;
        isTrackingDown = false;
        isTrackingUp = false;
    }
}