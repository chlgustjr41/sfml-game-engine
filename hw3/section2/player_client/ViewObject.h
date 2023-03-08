#include <SFML/Graphics.hpp>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "GameObject.h"
#include "MovableObject.h"

#ifndef VIEWOBJECT_H
#define VIEWOBJECT_H

class ViewObject : public GameObject
{
public:
    explicit ViewObject(sf::RectangleShape *initialObjectShape,
                        sf::RectangleShape *gameCharacter);

    void render(sf::RenderWindow *window);

    void updateView(sf::RenderWindow *window);

    std::string getType();
    void setType(std::string newType);

private:
};

#endif