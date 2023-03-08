#include <SFML/Graphics.hpp>
#include "HorizontallyMovingPlatformObject.h"
#include "HorizontallyMovingPlatformMovableObject.h"
#include "MovableObject.h"
#include "RenderableObject.h"

HorizontallyMovingPlatformObject::HorizontallyMovingPlatformObject(sf::RectangleShape *initialObjectShape, double movementLength)
    : GameObject(GameObject::HORIZONTALLY_MOVING_PLATFORM_OBJECT_TYPE,
                 new RenderableObject(initialObjectShape),
                 new HorizontallyMovingPlatformMovableObject(GameObject::HORIZONTALLY_MOVING_PLATFORM_OBJECT_TYPE,
                                                             initialObjectShape, movementLength))
{
}

void HorizontallyMovingPlatformObject::updateDuration(double duration)
{
    GameObject::getMovableObject()->setDuration(duration);
}

void HorizontallyMovingPlatformObject::render(sf::RenderWindow *window)
{
    static_cast<HorizontallyMovingPlatformMovableObject *>(GameObject::getMovableObject())->run();

    GameObject::getRenderableObject()->render(window);
}

std::string HorizontallyMovingPlatformObject::getType()
{
    return GameObject::getType();
}
void HorizontallyMovingPlatformObject::setType(std::string newType)
{
    GameObject::setType(newType);
}