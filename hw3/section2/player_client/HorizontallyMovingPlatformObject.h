#include <SFML/Graphics.hpp>
#include "GameObject.h"

#ifndef HORIZONTALLYMOVINGPLATFORMOBJECT_H
#define HORIZONTALLYMOVINGPLATFORMOBJECT_H

class HorizontallyMovingPlatformObject : public GameObject
{
public:
    explicit HorizontallyMovingPlatformObject(sf::RectangleShape *initialObjectShape, int initialIndex, double movementLength);

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