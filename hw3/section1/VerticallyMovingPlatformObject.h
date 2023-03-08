#include <SFML/Graphics.hpp>
#include "GameObject.h"

#ifndef VERTICALLYMOVINGPLATFORMOBJECT_H
#define VERTICALLYMOVINGPLATFORMOBJECT_H

class VerticallyMovingPlatformObject : public GameObject
{
public:
    explicit VerticallyMovingPlatformObject(sf::RectangleShape *initialObjectShape, double movementLength);

    void updateDuration(double duration);

    void render(sf::RenderWindow *window);

    std::string getType();
    void setType(std::string newType);

private:
};

#endif