#ifndef VERTICALLYMOVINGPLATFORMOBJECT_H
#define VERTICALLYMOVINGPLATFORMOBJECT_H

#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "MovableObject.h"
#include "VerticallyMovingPlatformMovableObject.h"
#include "RenderableObject.h"
#include "MovingPlatformNetworkObject.h"

class VerticallyMovingPlatformObject : public GameObject
{
public:
    explicit VerticallyMovingPlatformObject(EventManager *eventManager,
                                            sf::RectangleShape *initialObjectShape,
                                            double movementLength);

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