#ifndef MOVABLEOBJECT_H
#define MOVABLEOBJECT_H

#include <SFML/Graphics.hpp>
#include "EventManager.h"

class MovableObject
{

public:
    explicit MovableObject(EventManager *initialEventManager,
                           std::string objectType,
                           int objectIndex,
                           sf::RectangleShape *initialMovingObject);
    virtual void run() = 0;

    EventManager *getEventManager();
    std::string getType();
    int getIndex();
    sf::RectangleShape *getMovingObject();
    double getDuration();

    void setEventManager(EventManager *newEventManager);
    void setType(std::string newType);
    void setIndex(int newIndex);
    void setMovingObject(sf::RectangleShape *newMovingObject);
    void setDuration(double newDuration);

    std::string getHeader();

private:
    EventManager *eventManager;

    std::string type;
    int index;
    double duration;
    sf::RectangleShape *movingObject;
};

#endif