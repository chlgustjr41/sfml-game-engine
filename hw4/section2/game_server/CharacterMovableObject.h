#ifndef CHARACTERMOVABLEOBJECT_H
#define CHARACTERMOVABLEOBJECT_H

#include <SFML/Graphics.hpp>
#include <cmath>
#include <string>
#include <sstream>
#include "GameObject.h"
#include "MovableObject.h"
#include "VerticallyMovingPlatformMovableObject.h"
#include "HorizontallyMovingPlatformMovableObject.h"
#include "EventObject.h"

class CharacterMovableObject : public MovableObject
{
public:
    static const double CHARACTER_MOVEMENT;
    static const int JUMP_HEIGHT;
    static const int JUMP_FRAME;

    explicit CharacterMovableObject(EventManager *eventManager,
                                    std::string objectType,
                                    int objectIndex,
                                    sf::RectangleShape *movableobject,
                                    sf::RectangleShape *initialBoundary,
                                    std::vector<GameObject *> *initialCollidableObjects);

    bool getIsJumping();
    sf::RectangleShape *getBoundary();

    void run();

    void checkInputEvent();
    void checkPlatformCollideEvent();

private:
    std::vector<GameObject *> *collidableObjects;

    bool isMovingLeft;
    bool isMovingRight;

    bool isOnMovingPlatform;
    bool isJumping;
    double jumpTime;

    sf::RectangleShape *boundary;

    double velocity;

    int isIntercected();
    void split(std::string str, char deliminer, std::vector<std::string> &splitted);
};

#endif