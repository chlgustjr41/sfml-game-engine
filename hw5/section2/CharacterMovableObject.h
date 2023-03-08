#ifndef CHARACTERMOVABLEOBJECT_H
#define CHARACTERMOVABLEOBJECT_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <string>
#include <sstream>
#include "GameObject.h"
#include "MovableObject.h"
#include "EventObject.h"

class CharacterMovableObject : public MovableObject
{
public:
    static const double CHARACTER_MOVEMENT;

    explicit CharacterMovableObject(EventManager *eventManager,
                                    std::string objectType,
                                    int objectIndex,
                                    sf::RectangleShape *movableobject,
                                    sf::Vector2f objectSpawningPoint);
    bool getDuration();
    bool getIsMovingLeft();
    bool getIsMovingRight();

    void setDuration(double newDuration);
    void setIsMovingLeft(bool flag);
    void setIsMovingRight(bool flag);

    void run();

    void checkInputEvent();
    void checkDeathEvent();

private:
    bool isMovingLeft;
    bool isMovingRight;

    double duration;

    double velocity;

    sf::Vector2f spawningPoint;

    void split(std::string str, char deliminer, std::vector<std::string> &splitted);
};

#endif