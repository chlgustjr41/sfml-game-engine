#include "VerticallyMovingPlatformObject.h"

VerticallyMovingPlatformObject::VerticallyMovingPlatformObject(EventManager *eventManager,
                                                               sf::RectangleShape *initialObjectShape,
                                                               int initialIndex,
                                                               double movementLength)
    : GameObject(GameObject::VERTICALLY_MOVING_PLATFORM_OBJECT_TYPE,
                 new RenderableObject(initialObjectShape),
                 new VerticallyMovingPlatformMovableObject(eventManager,
                                                           GameObject::VERTICALLY_MOVING_PLATFORM_OBJECT_TYPE,
                                                           initialIndex,
                                                           initialObjectShape,
                                                           movementLength),
                 new MovingPlatformNetworkObject(GameObject::VERTICALLY_MOVING_PLATFORM_OBJECT_TYPE,
                                                 initialIndex,
                                                 initialObjectShape))
{
}

void VerticallyMovingPlatformObject::updateDuration(double duration)
{
    GameObject::getMovableObject()->setDuration(duration);
}

void VerticallyMovingPlatformObject::render(sf::RenderWindow *window)
{
    GameObject::getRenderableObject()->render(window);
}

void VerticallyMovingPlatformObject::run()
{
    static_cast<VerticallyMovingPlatformMovableObject *>(GameObject::getMovableObject())->run();
}

void VerticallyMovingPlatformObject::receiveNetworkMessage(std::string message)
{
    static_cast<MovingPlatformNetworkObject *>(GameObject::getNetworkObject())->receiveNetworkMessage(message);
}
std::string VerticallyMovingPlatformObject::createNetworkMessage()
{
    return static_cast<MovingPlatformNetworkObject *>(GameObject::getNetworkObject())->createNetworkMessage();
}
std::string VerticallyMovingPlatformObject::getHeader()
{
    return static_cast<MovingPlatformNetworkObject *>(GameObject::getNetworkObject())->getHeader();
}

std::string VerticallyMovingPlatformObject::getType()
{
    return GameObject::getType();
}
void VerticallyMovingPlatformObject::setType(std::string newType)
{
    GameObject::setType(newType);
}