#ifndef CHARACTEROBJECT_H
#define CHARACTEROBJECT_H

#include <utility>
#include <v8.h>
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "CharacterMovableObject.h"
#include "RenderableObject.h"
#include "v8helpers.h"

class CharacterObject : public GameObject
{
public:
    const static std::vector<sf::Color> CHARACTER_COLORS;
    const static float CHARACTER_SIZE;

    explicit CharacterObject(EventManager *eventManager,
                             std::string objectType,
                             sf::RectangleShape *initialObjectShape,
                             sf::Vector2f objectSpawningPoint);

    std::string getHeader();
    RenderableObject *getRenderableObject();
    MovableObject *getMovableObject();
    NetworkObject *getNetworkObject();
    sf::Vector2f getObjectPosition();

    void checkInputEvent();

    void updateDuration(double duration);

    void render(sf::RenderWindow *window);

    void run();

    v8::Local<v8::Object> exposeToV8(v8::Isolate *isolate, v8::Local<v8::Context> &context, std::string context_name);
    static void setGameObjectDuration(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info);
    static void getGameObjectDuration(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info);
    static void setGameObjectIsMovingLeft(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info);
    static void getGameObjectIsMovingLeft(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info);
    static void setGameObjectIsMovingRight(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info);
    static void getGameObjectIsMovingRight(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info);
};

#endif