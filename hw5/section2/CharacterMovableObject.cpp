#include "CharacterMovableObject.h"

const double CharacterMovableObject::CHARACTER_MOVEMENT = 15;

CharacterMovableObject::CharacterMovableObject(EventManager *eventManager,
                                               std::string objectType,
                                               int objectIndex,
                                               sf::RectangleShape *movableobject,
                                               sf::Vector2f objectSpawningPoint)
    : MovableObject(eventManager, objectType, objectIndex, movableobject)
{
    isMovingLeft = false;
    isMovingRight = false;

    velocity = CHARACTER_MOVEMENT;

    spawningPoint = objectSpawningPoint;

    duration = 0;
}

bool CharacterMovableObject::getDuration()
{
    return duration;
}
bool CharacterMovableObject::getIsMovingLeft()
{
    return isMovingLeft;
}
bool CharacterMovableObject::getIsMovingRight()
{
    return isMovingRight;
}

void CharacterMovableObject::setDuration(double newDuration)
{
    duration = newDuration;
}
void CharacterMovableObject::setIsMovingLeft(bool flag)
{
    isMovingLeft = flag;
}
void CharacterMovableObject::setIsMovingRight(bool flag)
{
    isMovingRight = flag;
}

void CharacterMovableObject::checkInputEvent()
{
    EventObject *currentEvent = MovableObject::getEventManager()->pollEvent(EventObject::CHARACTER_INPUT,
                                                                            MovableObject::getHeader());

    if (currentEvent != NULL && currentEvent->getContent() != "")
    {
        std::vector<std::string> tokenizedInputs;
        split(currentEvent->getContent(), ',', tokenizedInputs);

        for (int i = 0; i < tokenizedInputs.size(); i++)
        {
            if (tokenizedInputs[i] == "Left")
            {
                isMovingLeft = true;
            }
            else
            {
                isMovingLeft = false;
            }

            if (tokenizedInputs[i] == "Right")
            {
                isMovingRight = true;
            }
            else
            {
                isMovingRight = false;
            }

            if (tokenizedInputs[i] == "Space")
            {
                MovableObject::getEventManager()->registerEvent(new EventObject(EventObject::PROJECTILE_LAUNCH,
                                                                                EventManager::PROJECTILE_LAUNCH_PRIORITY,
                                                                                getHeader(),
                                                                                ""));
            }
        }
    }
    else
    {
        isMovingLeft = false;
        isMovingRight = false;
        return;
    }
}
void CharacterMovableObject::checkDeathEvent()
{
}

void CharacterMovableObject::split(std::string str, char deliminer, std::vector<std::string> &splitted)
{
    std::stringstream streamMsg(str);
    std::string token;
    while (std::getline(streamMsg, token, deliminer))
    {
        splitted.push_back(token);
    }
}

void CharacterMovableObject::run() {}