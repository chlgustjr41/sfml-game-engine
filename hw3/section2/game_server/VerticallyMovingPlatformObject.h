#include <SFML/Graphics.hpp>
#include "GameObject.h"

#ifndef VERTICALLYMOVINGPLATFORMOBJECT_H
#define VERTICALLYMOVINGPLATFORMOBJECT_H

class VerticallyMovingPlatformObject : public GameObject
{
public:
    explicit VerticallyMovingPlatformObject(sf::RectangleShape *initialObjectShape, int initialIndex, double movementLength);

    void updateDuration(double duration);

    void render(sf::RenderWindow *window);

    void run();

    void receiveNetworkMessage(std::string message);
    std::string createNetworkMessage();
    std::string getHeader();

    std::string getType();
    void setType(std::string newType);

private:
};

#endif