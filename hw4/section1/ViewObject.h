#ifndef VIEWOBJECT_H
#define VIEWOBJECT_H

#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "MovableObject.h"
#include "ViewMovableObject.h"
#include "RenderableObject.h"

class ViewObject : public GameObject
{
public:
    explicit ViewObject(EventManager *eventManager,
                        sf::RectangleShape *initialObjectShape,
                        sf::RectangleShape *gameCharacter,
                        int characterIndex);

    void render(sf::RenderWindow *window);

    void updateView(sf::RenderWindow *window);

    std::string getType();
    void setType(std::string newType);

private:
};

#endif