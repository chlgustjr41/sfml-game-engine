#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "MovableObject.h"

#ifndef CHARACTERMOVABLEOBJECT_H
#define CHARACTERMOVABLEOBJECT_H

class CharacterMovableObject : public MovableObject
{
public:
    static const double CHARACTER_MOVEMENT;
    static const int JUMP_HEIGHT;
    static const int JUMP_FRAME;

    explicit CharacterMovableObject(std::string objectType,
                                    sf::RectangleShape *movableobject,
                                    sf::RectangleShape *initialBoundary,
                                    std::vector<GameObject *> *initialCollidableObjects);

    bool getIsJumping();
    sf::RectangleShape *getBoundary();

    void run();

    void receiveInput(std::string inputs = "");

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