#ifndef CHARACTEROBJECT_H
#define CHARACTEROBJECT_H

#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "CharacterMovableObject.h"
#include "RenderableObject.h"
#include "CharacterNetworkObject.h"

class CharacterObject : public GameObject
{
public:
    explicit CharacterObject(EventManager *eventManager,
                             sf::RectangleShape *initialObjectShape,
                             sf::RectangleShape *boundary,
                             int initialIndex,
                             sf::Color initialColor = sf::Color::White,
                             std::vector<GameObject *> *initialCollidableObjects = NULL);

    void checkInputEvent();
    void checkPlatformCollideEvent();

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