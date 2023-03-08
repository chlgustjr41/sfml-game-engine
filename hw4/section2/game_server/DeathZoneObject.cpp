#include "DeathZoneObject.h"

DeathZoneObject::DeathZoneObject(EventManager *initialEventManager,
                                 sf::RectangleShape *initialObjectShape,
                                 sf::RectangleShape *initialSpawnPoint,
                                 std::vector<CharacterObject *> *initialCharacters)
    : GameObject(GameObject::DEATH_ZONE_OBJECT_TYPE, new RenderableObject(initialObjectShape))
{
    eventManager = initialEventManager;
    spawnPoint = initialSpawnPoint;
    characters = initialCharacters;
}
void DeathZoneObject::registerDeathEvent()
{
    for (int i = 0; i < characters->size(); i++)
    {
        if (GameObject::getRenderableObject()->getObjectShape()->getGlobalBounds().intersects((*characters)[i]->getRenderableObject()->getObjectShape()->getGlobalBounds()))
        {
            eventManager->registerEvent(new EventObject(EventObject::CHARACTER_DEATH,
                                                        EventManager::CHARACTER_DEATH_PRIORITY,
                                                        (*characters)[i]->getHeader(),
                                                        ""));
        }
    }
}
void DeathZoneObject::checkDeathEvent()
{
    registerDeathEvent();
    for (int i = 0; i < characters->size(); i++)
    {
        if (eventManager->pollEvent(EventObject::CHARACTER_DEATH, (*characters)[i]->getHeader()) != NULL)
        {

            eventManager->registerEvent(new EventObject(EventObject::CHARACTER_SPAWN,
                                                        EventManager::CHARACTER_SPAWN_PRIORITY,
                                                        (*characters)[i]->getHeader(),
                                                        std::to_string((*characters)[i]->getRenderableObject()->getObjectShape()->getFillColor().toInteger())));
        }
    }
}
void DeathZoneObject::checkSpawnEvent()
{
    for (int i = 0; i < characters->size(); i++)
    {
        if (eventManager->pollEvent(EventObject::CHARACTER_SPAWN, (*characters)[i]->getHeader()) != NULL)
        {
            (*characters)[i]->getRenderableObject()->getObjectShape()->setPosition(spawnPoint->getPosition().x, spawnPoint->getPosition().y);
        }
    }
}

void DeathZoneObject::render(sf::RenderWindow *window)
{
    GameObject::getRenderableObject()->render(window);
    window->draw(*spawnPoint);
}