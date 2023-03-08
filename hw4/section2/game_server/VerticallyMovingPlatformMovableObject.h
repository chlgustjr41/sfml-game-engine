#ifndef VERTICALLYMOVINGPLATFORMMOVABLEOBJECT_H
#define VERTICALLYMOVINGPLATFORMMOVABLEOBJECT_H

#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "MovableObject.h"

class VerticallyMovingPlatformMovableObject : public MovableObject
{
public:
    static const double MOVING_PLATFORM_MOVEMENT;

    explicit VerticallyMovingPlatformMovableObject(EventManager *eventManager,
                                                   std::string objectType,
                                                   int objectIndex,
                                                   sf::RectangleShape *movableobject,
                                                   double initialMovementLength);

    void run();

    void attach(sf::RectangleShape *object);
    bool updateAttach(sf::RectangleShape *object);

    bool getDirectionFlag();
    double getOriginalXPosition();
    double getMovementLength();

    void updateOriginalXPosition();
    void setMovementLength(double newMovementLength);

private:
    std::vector<sf::RectangleShape *> attachedObjects;
    bool directionFlag;
    double originalXPosition;
    double movementLength;
};

#endif