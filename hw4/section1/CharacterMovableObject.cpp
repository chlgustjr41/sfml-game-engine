#include "CharacterMovableObject.h"

const double CharacterMovableObject::CHARACTER_MOVEMENT = 1.25;
const int CharacterMovableObject::JUMP_HEIGHT = 3;
const int CharacterMovableObject::JUMP_FRAME = 45;

CharacterMovableObject::CharacterMovableObject(EventManager *eventManager,
                                               std::string objectType,
                                               int objectIndex,
                                               sf::RectangleShape *movableobject,
                                               sf::RectangleShape *initialBoundary,
                                               std::vector<GameObject *> *initialCollidableObjects)
    : MovableObject(eventManager, objectType, objectIndex, movableobject)
{
    collidableObjects = initialCollidableObjects;

    isMovingLeft = false;
    isMovingRight = false;

    isOnMovingPlatform = false;
    isJumping = false;
    jumpTime = 0;

    velocity = CHARACTER_MOVEMENT;

    boundary = initialBoundary;
}

bool CharacterMovableObject::getIsJumping()
{
    return isJumping;
}
sf::RectangleShape *CharacterMovableObject::getBoundary()
{
    return boundary;
}

void CharacterMovableObject::run()
{
    sf::RectangleShape *thisObject = MovableObject::getMovingObject();
    double duration = MovableObject::getDuration();

    // Character left movement
    if (isMovingLeft && thisObject->getPosition().x >= boundary->getPosition().x)
    {
        thisObject->move(-velocity * duration * GameObject::TIME_RATIO, 0);
    }
    // Character right movement
    if (isMovingRight && thisObject->getPosition().x + thisObject->getSize().x <= boundary->getPosition().x + boundary->getSize().x)
    {
        thisObject->move(+velocity * duration * GameObject::TIME_RATIO, 0);
    }

    int intersectingObjectIndex = isIntercected();
    // Free falling
    if (!isJumping &&
        intersectingObjectIndex == -1)
    {
        isJumping = true;
        jumpTime = JUMP_FRAME / 2;
    }
    // Character jumping movement
    if (isJumping)
    {
        // Jump logic for smooth jumping with different acceleration
        thisObject->move(0, -JUMP_HEIGHT * std::cos(M_PI * jumpTime / JUMP_FRAME) * duration * GameObject::TIME_RATIO);
        if (jumpTime < JUMP_FRAME)
        {
            jumpTime += 1;
        }
        if (intersectingObjectIndex != -1)
        {
            std::string intersectingObjectType = (*collidableObjects)[intersectingObjectIndex]->getType();
            sf::RectangleShape *intersectingObject = (*collidableObjects)[intersectingObjectIndex]->getRenderableObject()->getObjectShape();
            // Landing on top of a non-moving platform
            if (intersectingObjectType == GameObject::STATIC_PLATFORM_OBJECT_TYPE &&
                thisObject->getPosition().y < intersectingObject->getPosition().y)
            {
                isJumping = false;
                jumpTime = 0;
                thisObject->setPosition(thisObject->getPosition().x, intersectingObject->getPosition().y - thisObject->getSize().y + 0.5); // 0.5 buffer to intersect
            }
            // Hitting the moving platform (ceiling) while jumping
            else if (thisObject->getPosition().y > intersectingObject->getPosition().y)
            {
                MovableObject::getEventManager()->registerEvent(new EventObject(EventObject::CHARACTER_PLATFORM_COLLIDE,
                                                                                EventManager::CHARACTER_PLATFORM_COLLIDE_PRIORITY,
                                                                                MovableObject::getHeader(),
                                                                                "BounceOff"));
            }
            // Landing on top of a vertically moving platform
            else if (intersectingObjectType == GameObject::VERTICALLY_MOVING_PLATFORM_OBJECT_TYPE &&
                     thisObject->getPosition().y < intersectingObject->getPosition().y)
            {
                MovableObject::getEventManager()->registerEvent(new EventObject(EventObject::CHARACTER_PLATFORM_COLLIDE,
                                                                                EventManager::CHARACTER_PLATFORM_COLLIDE_PRIORITY,
                                                                                MovableObject::getHeader(),
                                                                                "Attached"));
            }
            // Landing on top of a horizontally moving platform
            else if (intersectingObjectType == GameObject::HORIZONTALLY_MOVING_PLATFORM_OBJECT_TYPE &&
                     thisObject->getPosition().y < intersectingObject->getPosition().y)
            {
                isOnMovingPlatform = true;
                static_cast<HorizontallyMovingPlatformMovableObject *>((*collidableObjects)[intersectingObjectIndex]->getMovableObject())->attach(thisObject);

                isJumping = false;
                jumpTime = 0;

                thisObject->setPosition(thisObject->getPosition().x, intersectingObject->getPosition().y - thisObject->getSize().y + 0.1); // 0.1 buffer to intersect
            }
        }
    }
}

void CharacterMovableObject::checkInputEvent()
{
    EventObject *currentEvent =
        MovableObject::getEventManager()->pollEvent(EventObject::CHARACTER_INPUT,
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

            if (tokenizedInputs[i] == "Space" && !isJumping)
            {
                isJumping = true;
                MovableObject::getMovingObject()->move(0, -0.5); // Getting off from the intercept state from the ground
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
void CharacterMovableObject::checkPlatformCollideEvent()
{
    EventObject *currentEvent = MovableObject::getEventManager()->pollEvent(EventObject::CHARACTER_PLATFORM_COLLIDE,
                                                                            MovableObject::getHeader());

    if (currentEvent != NULL)
    {
        sf::RectangleShape *thisObject = MovableObject::getMovingObject();

        int intersectingObjectIndex = isIntercected();
        sf::RectangleShape *intersectingObject = (*collidableObjects)[intersectingObjectIndex]->getRenderableObject()->getObjectShape();

        if (currentEvent->getContent() == "Attached")
        {
            isOnMovingPlatform = true;
            static_cast<VerticallyMovingPlatformMovableObject *>((*collidableObjects)[intersectingObjectIndex]->getMovableObject())->attach(thisObject);

            isJumping = false;
            jumpTime = 0;

            thisObject->setPosition(thisObject->getPosition().x, intersectingObject->getPosition().y - thisObject->getSize().y + 0.1); // 0.1 buffer to intersect
        }
        else if (currentEvent->getContent() == "BounceOff")
        {
            // 5 for bouncing out and manually separating two contacting objects
            MovableObject::getMovingObject()->setPosition(thisObject->getPosition().x, intersectingObject->getPosition().y + intersectingObject->getSize().y + 5);
            // falling + bounce out
            jumpTime = JUMP_FRAME / 2 + 5;
        }
    }
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

int CharacterMovableObject::isIntercected()
{
    for (int i = 0; i < collidableObjects->size(); i++)
    {
        if (MovableObject::getMovingObject()->getGlobalBounds().intersects((*collidableObjects)[i]->getRenderableObject()->getObjectShape()->getGlobalBounds()))
        {
            return i;
        }
    }

    isOnMovingPlatform = false;
    return -1;
}