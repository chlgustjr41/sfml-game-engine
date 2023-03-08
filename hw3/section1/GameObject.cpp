#include <string>
#include "GameObject.h"
#include "RenderableObject.h"
#include "MovableObject.h"

const int GameObject::WINDOW_WIDTH = 800;
const int GameObject::WINDOW_HEIGHT = 600;

const double GameObject::TIME_RATIO = 0.5;

const std::string GameObject::CHARACTER_OBJECT_TYPE = "character";
const std::string GameObject::STATIC_PLATFORM_OBJECT_TYPE = "staticPlatform";
const std::string GameObject::VERTICALLY_MOVING_PLATFORM_OBJECT_TYPE = "verticallyMovingPlatform";
const std::string GameObject::HORIZONTALLY_MOVING_PLATFORM_OBJECT_TYPE = "horizontallyMovingPlatform";
const std::string GameObject::DEATH_ZONE_OBJECT_TYPE = "deathZone";
const std::string GameObject::VIEW_OBJECT_TYPE = "view";

GameObject::GameObject(std::string objectType,
                       RenderableObject *intialRenderObject,
                       MovableObject *initialMoveableObject)
{
    type = objectType;
    renderObject = intialRenderObject;
    movableObject = initialMoveableObject;
}

std::string GameObject::getType()
{
    return type;
}
RenderableObject *GameObject::getRenderableObject()
{
    return renderObject;
}
MovableObject *GameObject::getMovableObject()
{
    return movableObject;
}

void GameObject::setType(std::string str)
{
    type = str;
}
void GameObject::setRenderableObject(RenderableObject *newRenderObject)
{
    renderObject = newRenderObject;
}
void GameObject::setMovableObject(MovableObject *newMovableObject)
{
    movableObject = newMovableObject;
}
