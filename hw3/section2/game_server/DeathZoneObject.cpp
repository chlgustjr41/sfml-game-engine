#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "DeathZoneObject.h"
#include "CharacterObject.h"
#include "RenderableObject.h"

DeathZoneObject::DeathZoneObject(sf::RectangleShape *initialObjectShape,
                                 sf::RectangleShape *initialSpawnPoint,
                                 std::vector<CharacterObject *> *initialCharacters)
    : GameObject(GameObject::DEATH_ZONE_OBJECT_TYPE, new RenderableObject(initialObjectShape))
{
    spawnPoint = initialSpawnPoint;
    characters = initialCharacters;
}

void DeathZoneObject::checkDeathZone()
{
    for (int i = 0; i < characters->size(); i++)
    {
        if (GameObject::getRenderableObject()->getObjectShape()->getGlobalBounds().intersects((*characters)[i]->getRenderableObject()->getObjectShape()->getGlobalBounds()))
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