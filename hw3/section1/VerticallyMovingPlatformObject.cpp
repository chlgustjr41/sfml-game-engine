#include <SFML/Graphics.hpp>
#include "VerticallyMovingPlatformObject.h"
#include "VerticallyMovingPlatformMovableObject.h"
#include "MovableObject.h"
#include "RenderableObject.h"

VerticallyMovingPlatformObject::VerticallyMovingPlatformObject(sf::RectangleShape *initialObjectShape, double movementLength)
    : GameObject(GameObject::VERTICALLY_MOVING_PLATFORM_OBJECT_TYPE,
                 new RenderableObject(initialObjectShape),
                 new VerticallyMovingPlatformMovableObject(GameObject::VERTICALLY_MOVING_PLATFORM_OBJECT_TYPE,
                                                           initialObjectShape, movementLength))
{
}

void VerticallyMovingPlatformObject::updateDuration(double duration)
{
    GameObject::getMovableObject()->setDuration(duration);
}

void VerticallyMovingPlatformObject::render(sf::RenderWindow *window)
{
    static_cast<VerticallyMovingPlatformMovableObject *>(GameObject::getMovableObject())->run();

    GameObject::getRenderableObject()->render(window);
}

std::string VerticallyMovingPlatformObject::getType()
{
    return GameObject::getType();
}
void VerticallyMovingPlatformObject::setType(std::string newType)
{
    GameObject::setType(newType);
}