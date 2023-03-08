#ifndef HORIZONTALLYMOVINGPLATFORMMOVABLEOBJECT_H
#define HORIZONTALLYMOVINGPLATFORMMOVABLEOBJECT_H

#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "MovableObject.h"

class HorizontallyMovingPlatformMovableObject : public MovableObject
{
public:
    static const double MOVING_PLATFORM_MOVEMENT;

    explicit HorizontallyMovingPlatformMovableObject(EventManager *eventManager,
                                                     std::string objectType,
                                                     int objectIndex,
                                                     sf::RectangleShape *movableobject,
                                                     double initialMovementLength);

    void run();

    void attach(sf::RectangleShape *object);
    bool updateAttach(sf::RectangleShape *object);

    bool getDirectionFlag();
    double getOriginalYPosition();
    double getMovementLength();

    void updateOriginalYPosition();
    void setMovementLength(double newMovementLength);

private:
    std::vector<sf::RectangleShape *> attachedObjects;
    bool directionFlag;
    double originalYPosition;
    double movementLength;
};

#endif