#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "CharacterObject.h"

#ifndef DEATHZONEOBJECT_H
#define DEATHZONEOBJECT_H

class DeathZoneObject : public GameObject
{
public:
    explicit DeathZoneObject(sf::RectangleShape *initialObjectShape,
                             sf::RectangleShape *initialSpawnPoint,
                             std::vector<CharacterObject *> *initialCharacters);

    void checkDeathZone();

    void render(sf::RenderWindow *window);

private:
    sf::RectangleShape *spawnPoint;
    std::vector<CharacterObject *> *characters;
};

#endif