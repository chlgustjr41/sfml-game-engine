#include "CharacterObject.h"

CharacterObject::CharacterObject(EventManager *eventManager,
                                 sf::RectangleShape *initialObjectShape,
                                 sf::RectangleShape *boundary,
                                 int initialIndex,
                                 sf::Color initialColor,
                                 std::vector<GameObject *> *initialCollidableObjects)
    : GameObject(GameObject::CHARACTER_OBJECT_TYPE,
                 new RenderableObject(initialObjectShape),
                 new CharacterMovableObject(eventManager,
                                            GameObject::CHARACTER_OBJECT_TYPE,
                                            initialIndex, initialObjectShape,
                                            boundary,
                                            initialCollidableObjects),
                 new CharacterNetworkObject(GameObject::CHARACTER_OBJECT_TYPE,
                                            initialIndex,
                                            initialObjectShape,
                                            initialColor))
{
}

void CharacterObject::checkInputEvent()
{
    static_cast<CharacterMovableObject *>(GameObject::getMovableObject())->checkInputEvent();
}
void CharacterObject::checkPlatformCollideEvent()
{
    static_cast<CharacterMovableObject *>(GameObject::getMovableObject())->checkPlatformCollideEvent();
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
