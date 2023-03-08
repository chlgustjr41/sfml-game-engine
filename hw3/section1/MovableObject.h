#include <SFML/Graphics.hpp>
#include "Timeline.h"

#ifndef MOVABLEOBJECT_H
#define MOVABLEOBJECT_H

class MovableObject
{

public:
    explicit MovableObject(std::string objectType, sf::RectangleShape *initialMovingObject);
    virtual void run() = 0;

    std::string getType();
    sf::RectangleShape *getMovingObject();
    double getDuration();

    void setType(std::string newType);
    void setMovingObject(sf::RectangleShape *newMovingObject);
    void setDuration(double newDuration);

private:
    std::string type;
    double duration;
    sf::RectangleShape *movingObject;
};

#endif