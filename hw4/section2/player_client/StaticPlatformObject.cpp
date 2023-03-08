#include "StaticPlatformObject.h"

StaticPlatformObject::StaticPlatformObject(sf::RectangleShape *initialObjectShape)
    : GameObject(GameObject::STATIC_PLATFORM_OBJECT_TYPE, new RenderableObject(initialObjectShape))
{
}

void StaticPlatformObject::render(sf::RenderWindow *window)
{
    GameObject::getRenderableObject()->render(window);
}