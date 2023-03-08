#include <SFML/Graphics.hpp>
#include "RenderableObject.h"

RenderableObject::RenderableObject(sf::RectangleShape *initialObjectShape)
{
    objectShape = initialObjectShape;
}

sf::RectangleShape *RenderableObject::getObjectShape()
{
    return objectShape;
}

void RenderableObject::setObjectShape(sf::RectangleShape *newObjectShape)
{
    objectShape = newObjectShape;
}

void RenderableObject::render(sf::RenderWindow *window)
{
    window->draw(*objectShape);
}