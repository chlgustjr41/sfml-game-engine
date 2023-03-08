#include "ProjectileMovableObject.h"

const double ProjectileMovableObject::PROJECTILE_MOVEMENT = 8;

ProjectileMovableObject::ProjectileMovableObject(EventManager *eventManager,
                                                 std::string objectType,
                                                 int objectIndex,
                                                 sf::RectangleShape *movableobject,
                                                 std::vector<GameObject *> *objectTargets)
    : MovableObject(eventManager, objectType, objectIndex, movableobject)
{
    velocity = PROJECTILE_MOVEMENT;

    duration = 0;
}

bool ProjectileMovableObject::getDuration()
{
    return duration;
}

void ProjectileMovableObject::setDuration(double newDuration)
{
    duration = newDuration;
}

void ProjectileMovableObject::checkCollisionEvent()
{
}

void ProjectileMovableObject::run() {}