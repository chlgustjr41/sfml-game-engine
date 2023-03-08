#include <SFML/Graphics.hpp>
#include "HorizontallyMovingPlatformObject.h"
#include "HorizontallyMovingPlatformMovableObject.h"
#include "MovableObject.h"
#include "RenderableObject.h"
#include "MovingPlatformNetworkObject.h"

HorizontallyMovingPlatformObject::HorizontallyMovingPlatformObject(sf::RectangleShape *initialObjectShape, int initialIndex, double movementLength)
    : GameObject(GameObject::HORIZONTALLY_MOVING_PLATFORM_OBJECT_TYPE,
                 new RenderableObject(initialObjectShape),
                 new HorizontallyMovingPlatformMovableObject(GameObject::HORIZONTALLY_MOVING_PLATFORM_OBJECT_TYPE,
                                                             initialObjectShape, movementLength),
                 new MovingPlatformNetworkObject(GameObject::HORIZONTALLY_MOVING_PLATFORM_OBJECT_TYPE, initialIndex, initialObjectShape))
{
}

void HorizontallyMovingPlatformObject::updateDuration(double duration)
{
    GameObject::getMovableObject()->setDuration(duration);
}

void HorizontallyMovingPlatformObject::render(sf::RenderWindow *window)
{
    GameObject::getRenderableObject()->render(window);
}

void HorizontallyMovingPlatformObject::run()
{
    static_cast<HorizontallyMovingPlatformMovableObject *>(GameObject::getMovableObject())->run();
}

void HorizontallyMovingPlatformObject::receiveNetworkMessage(std::string message)
{
    static_cast<MovingPlatformNetworkObject *>(GameObject::getNetworkObject())->receiveNetworkMessage(message);
}
std::string HorizontallyMovingPlatformObject::createNetworkMessage()
{
    return static_cast<MovingPlatformNetworkObject *>(GameObject::getNetworkObject())->createNetworkMessage();
}
std::string HorizontallyMovingPlatformObject::getHeader()
{
    return static_cast<MovingPlatformNetworkObject *>(GameObject::getNetworkObject())->getHeader();
}

std::string HorizontallyMovingPlatformObject::getType()
{
    return GameObject::getType();
}
void HorizontallyMovingPlatformObject::setType(std::string newType)
{
    GameObject::setType(newType);
}