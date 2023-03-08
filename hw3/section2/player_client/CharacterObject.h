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
                             int initialIndex,
                             sf::Color initialColor = sf::Color::White,
                             std::vector<GameObject *> *initialCollidableObjects = NULL);

    void receiveInput(std::string inputs = "");

    void updateDuration(double duration);

    void render(sf::RenderWindow *window);

    void run();

    void receiveNetworkMessage(std::string message);
    std::string createNetworkMessage();
    std::string getHeader();

    std::string getType();
    int getIndex();
    bool getIsJumping();

    void setType(std::string newType);

private:
};

#endif