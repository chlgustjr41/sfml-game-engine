#ifndef PROJECTILEMOVABLEOBJECT_H
#define PROJECTILEMOVABLEOBJECT_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <string>
#include <sstream>
#include "GameObject.h"
#include "MovableObject.h"
#include "EventObject.h"
#include "CharacterObject.h"

class ProjectileMovableObject : public MovableObject
{
public:
    static const double PROJECTILE_MOVEMENT;

    explicit ProjectileMovableObject(EventManager *eventManager,
                                     std::string objectType,
                                     int objectIndex,
                                     sf::RectangleShape *movableobject,
                                     std::vector<GameObject *> *objectTargets);
    bool getDuration();

    void setDuration(double newDuration);

    void run();

    void checkCollisionEvent();

private:
    std::vector<CharacterObject *> *targets;

    double duration;

    double velocity;
};

#endif