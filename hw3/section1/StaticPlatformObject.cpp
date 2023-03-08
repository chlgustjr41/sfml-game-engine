#include <SFML/Graphics.hpp>

#include "StaticPlatformObject.h"
#include "CharacterMovableObject.h"
#include "MovableObject.h"
#include "RenderableObject.h"
#include "Timeline.h"

StaticPlatformObject::StaticPlatformObject(sf::RectangleShape *initialObjectShape)
    : GameObject(GameObject::STATIC_PLATFORM_OBJECT_TYPE, new RenderableObject(initialObjectShape))
{
}

void StaticPlatformObject::render(sf::RenderWindow *window)
{
    GameObject::getRenderableObject()->render(window);
}