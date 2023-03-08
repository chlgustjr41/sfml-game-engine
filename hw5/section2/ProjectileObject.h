#ifndef PROJECTILEOBJECT_H
#define PROJECTILEOBJECT_H

#include <utility>
#include <v8.h>
#include <SFML/Graphics.hpp>
#include <string>
#include "GameObject.h"
#include "CharacterObject.h"
#include "RenderableObject.h"
#include "ProjectileMovableObject.h"
#include "v8helpers.h"

class ProjectileObject : public GameObject
{
public:
    explicit ProjectileObject(EventManager *initialEventManager,
                              sf::RectangleShape *initialObjectShape,
                              std::vector<GameObject *> *initialCharacters,
                              std::string initialSourceType);

    void checkEnemyCollision();

    void render(sf::RenderWindow *window);

    std::string getSourceType();
    void setSourceType(std::string newSourceType);

    v8::Local<v8::Object> exposeToV8(v8::Isolate *isolate, v8::Local<v8::Context> &context, std::string context_name);
    static void setGameObjectSourceType(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info);
    static void getGameObjectSourceType(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info);

private:
    EventManager *eventManager;
    std::vector<GameObject *> *characters;
    std::string sourceType;
};

#endif