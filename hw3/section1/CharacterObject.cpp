#include <SFML/Graphics.hpp>

#include "CharacterObject.h"
#include "CharacterMovableObject.h"
#include "MovableObject.h"
#include "RenderableObject.h"
#include "Timeline.h"

CharacterObject::CharacterObject(sf::RectangleShape *initialObjectShape,
                                 sf::RectangleShape *boundary,
                                 std::vector<GameObject *> *initialCollidableObjects)
    : GameObject(GameObject::CHARACTER_OBJECT_TYPE, new RenderableObject(initialObjectShape), new CharacterMovableObject(GameObject::CHARACTER_OBJECT_TYPE, initialObjectShape, boundary, initialCollidableObjects))
{
}

void CharacterObject::receiveInput(std::string inputs)
{
    static_cast<CharacterMovableObject *>(GameObject::getMovableObject())->receiveInput(inputs);
}

void CharacterObject::updateDuration(double duration)
{
    GameObject::getMovableObject()->setDuration(duration);
}

void CharacterObject::render(sf::RenderWindow *window)
{
    static_cast<CharacterMovableObject *>(GameObject::getMovableObject())->run();

    GameObject::getRenderableObject()->render(window);
}

std::string CharacterObject::getType()
{
    return GameObject::getType();
}
bool CharacterObject::getIsJumping()
{
    return static_cast<CharacterMovableObject *>(GameObject::getMovableObject())->getIsJumping();
}

void CharacterObject::setType(std::string newType)
{
    GameObject::setType(newType);
}
