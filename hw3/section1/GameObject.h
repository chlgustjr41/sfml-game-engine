#include <SFML/Graphics.hpp>
#include <string>
#include "RenderableObject.h"
#include "MovableObject.h"

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

class GameObject
{
public:
    static const int WINDOW_WIDTH;
    static const int WINDOW_HEIGHT;

    static const double TIME_RATIO;

    static const std::string CHARACTER_OBJECT_TYPE;
    static const std::string STATIC_PLATFORM_OBJECT_TYPE;
    static const std::string VERTICALLY_MOVING_PLATFORM_OBJECT_TYPE;
    static const std::string HORIZONTALLY_MOVING_PLATFORM_OBJECT_TYPE;
    static const std::string DEATH_ZONE_OBJECT_TYPE;
    static const std::string VIEW_OBJECT_TYPE;

    explicit GameObject(std::string objectType,
                        RenderableObject *intialRenderObject = NULL,
                        MovableObject *initialMoveableObject = NULL);
    std::string getType();
    RenderableObject *getRenderableObject();
    MovableObject *getMovableObject();

    void setType(std::string str);
    void setRenderableObject(RenderableObject *newRenderObject);
    void setMovableObject(MovableObject *newMovableObject);

private:
    std::string type;

    RenderableObject *renderObject;
    MovableObject *movableObject;
};

#endif