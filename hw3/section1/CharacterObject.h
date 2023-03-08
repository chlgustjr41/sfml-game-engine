#include <SFML/Graphics.hpp>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "GameObject.h"
#include "MovableObject.h"
#include "Timeline.h"

#ifndef CHARACTEROBJECT_H
#define CHARACTEROBJECT_H

class CharacterObject : public GameObject
{
public:
    explicit CharacterObject(sf::RectangleShape *initialObjectShape,
                             sf::RectangleShape *boundary,
                             std::vector<GameObject *> *initialCollidableObjects = NULL);

    void receiveInput(std::string inputs = "");

    void updateDuration(double duration);

    void render(sf::RenderWindow *window);

    std::string getType();
    bool getIsJumping();

    void setType(std::string newType);

private:
};

#endif