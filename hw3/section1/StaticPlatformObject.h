#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "MovableObject.h"
#include "Timeline.h"

#ifndef STATICPLATFORMOBJECT_H
#define STATICPLATFORMOBJECT_H

class StaticPlatformObject : public GameObject
{
public:
    explicit StaticPlatformObject(sf::RectangleShape *initialObjectShape);

    void render(sf::RenderWindow *window);

private:
};

#endif