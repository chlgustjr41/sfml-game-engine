#ifndef ViewMOVABLEOBJECT_H
#define ViewMOVABLEOBJECT_H

#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "MovableObject.h"

class ViewMovableObject : public MovableObject
{
public:
    explicit ViewMovableObject(EventManager *eventManager,
                               std::string objectType,
                               int objectIndex,
                               sf::RectangleShape *initialObjectShape,
                               sf::RectangleShape *gameCharacter);

    void run();

    void updateView(sf::RenderWindow *window);

    sf::RectangleShape *getCharacter();
    bool getIsTrackingLeft();
    bool getIsTrackingRight();
    bool getIsTrackingDown();
    bool getIsTrackingUp();

private:
    sf::RectangleShape *character;
    bool isTrackingLeft;
    bool isTrackingRight;
    bool isTrackingDown;
    bool isTrackingUp;
};

#endif