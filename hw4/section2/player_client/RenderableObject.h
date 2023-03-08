#ifndef RENDERABLEOBJECT_H
#define RENDERABLEOBJECT_H

#include <SFML/Graphics.hpp>

class RenderableObject
{
public:
    explicit RenderableObject(sf::RectangleShape *objectShape);
    void render(sf::RenderWindow *window);

    sf::RectangleShape *getObjectShape();
    void setObjectShape(sf::RectangleShape *newObjectShape);

private:
    sf::RectangleShape *objectShape;
};

#endif