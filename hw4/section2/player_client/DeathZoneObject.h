#ifndef DEATHZONEOBJECT_H
#define DEATHZONEOBJECT_H

#include <SFML/Graphics.hpp>
#include <string>
#include "GameObject.h"
#include "CharacterObject.h"
#include "RenderableObject.h"

class DeathZoneObject : public GameObject
{
public:
    explicit DeathZoneObject(EventManager *initialEventManager,
                             sf::RectangleShape *initialObjectShape,
                             sf::RectangleShape *initialSpawnPoint,
                             std::vector<CharacterObject *> *initialCharacters);

    void checkDeathEvent();
    void checkSpawnEvent();

    void render(sf::RenderWindow *window);

private:
    EventManager *eventManager;
    sf::RectangleShape *spawnPoint;
    std::vector<CharacterObject *> *characters;

    void registerDeathEvent();
};

#endif