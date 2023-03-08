#include <SFML/Graphics.hpp>
#include "GameObject.h"

#ifndef HORIZONTALLYMOVINGPLATFORMOBJECT_H
#define HORIZONTALLYMOVINGPLATFORMOBJECT_H

class HorizontallyMovingPlatformObject : public GameObject
{
public:
    explicit HorizontallyMovingPlatformObject(sf::RectangleShape *initialObjectShape, double movementLength);

    void updateDuration(double duration);

    void render(sf::RenderWindow *window);

    std::string getType();
    void setType(std::string newType);

private:
};

#endif