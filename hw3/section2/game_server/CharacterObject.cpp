#include <SFML/Graphics.hpp>

#include "CharacterObject.h"
#include "CharacterMovableObject.h"
#include "MovableObject.h"
#include "RenderableObject.h"
#include "CharacterNetworkObject.h"

CharacterObject::CharacterObject(sf::RectangleShape *initialObjectShape,
                                 sf::RectangleShape *boundary,
                                 int initialIndex,
                                 sf::Color initialColor,
                                 std::vector<GameObject *> *initialCollidableObjects)
    : GameObject(GameObject::CHARACTER_OBJECT_TYPE,
                 new RenderableObject(initialObjectShape),
                 new CharacterMovableObject(GameObject::CHARACTER_OBJECT_TYPE, initialObjectShape, boundary, initialCollidableObjects),
                 new CharacterNetworkObject(GameObject::CHARACTER_OBJECT_TYPE, initialIndex, initialObjectShape, initialColor))
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
    GameObject::getRenderableObject()->render(window);
}

void CharacterObject::run()
{
    static_cast<CharacterMovableObject *>(GameObject::getMovableObject())->run();
}

void CharacterObject::receiveNetworkMessage(std::string message)
{
    static_cast<CharacterNetworkObject *>(GameObject::getNetworkObject())->receiveNetworkMessage(message);
}
std::string CharacterObject::createNetworkMessage()
{
    return static_cast<CharacterNetworkObject *>(GameObject::getNetworkObject())->createNetworkMessage();
}
std::string CharacterObject::getHeader()
{
    return static_cast<CharacterNetworkObject *>(GameObject::getNetworkObject())->getHeader();
}

std::string CharacterObject::getType()
{
    return GameObject::getType();
}
int CharacterObject::getIndex()
{
    return GameObject::getNetworkObject()->getIndex();
}
bool CharacterObject::getIsJumping()
{
    return static_cast<CharacterMovableObject *>(GameObject::getMovableObject())->getIsJumping();
}

void CharacterObject::setType(std::string newType)
{
    GameObject::setType(newType);
}
