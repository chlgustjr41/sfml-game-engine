#ifndef HORIZONTALLYMOVINGPLATFORMOBJECT_H
#define HORIZONTALLYMOVINGPLATFORMOBJECT_H

#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "MovableObject.h"
#include "HorizontallyMovingPlatformMovableObject.h"
#include "RenderableObject.h"
#include "MovingPlatformNetworkObject.h"

class HorizontallyMovingPlatformObject : public GameObject
{
public:
    explicit HorizontallyMovingPlatformObject(EventManager *eventManager,
                                              sf::RectangleShape *initialObjectShape,
                                              int initialIndex,
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