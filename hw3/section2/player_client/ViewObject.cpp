#include <SFML/Graphics.hpp>

#include "ViewObject.h"
#include "ViewMovableObject.h"
#include "MovableObject.h"
#include "RenderableObject.h"
#include "Timeline.h"

ViewObject::ViewObject(sf::RectangleShape *initialObjectShape,
                       sf::RectangleShape *gameCharacter)
    : GameObject(GameObject::CHARACTER_OBJECT_TYPE, new RenderableObject(initialObjectShape), new ViewMovableObject(GameObject::CHARACTER_OBJECT_TYPE, initialObjectShape, gameCharacter))
{
}
void ViewObject::render(sf::RenderWindow *window)
{
    GameObject::getRenderableObject()->render(window);
}

void ViewObject::updateView(sf::RenderWindow *window)
{
    ViewMovableObject *viewMovableObject = static_cast<ViewMovableObject *>(GameObject::getMovableObject());
    viewMovableObject->run();

    sf::View view = window->getView();
    if (viewMovableObject->getIsTrackingRight())
    {
        view.setCenter(viewMovableObject->getCharacter()->getPosition().x, view.getCenter().y);
        window->setView(view);
    }
    else if (viewMovableObject->getIsTrackingLeft())
    {
        view.setCenter(viewMovableObject->getCharacter()->getPosition().x + viewMovableObject->getCharacter()->getSize().x, view.getCenter().y);
        window->setView(view);
    }
}

std::string ViewObject::getType()
{
    return GameObject::getType();
}
void ViewObject::setType(std::string newType)
{
    GameObject::setType(newType);
}
