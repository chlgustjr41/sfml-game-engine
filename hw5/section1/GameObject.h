#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <utility>
#include <v8.h>
#include <SFML/Graphics.hpp>
#include <string>
#include "RenderableObject.h"
#include "MovableObject.h"
#include "NetworkObject.h"
#include "EventManager.h"

class GameObject
{
public:
    static EventManager *eventManager;
    static std::vector<GameObject *> *collidableObjects;
    static std::vector<GameObject *> *characterObjects;

    static int characterCount;
    static int staticPlatformCount;
    static int verticallyMovingPlatformCount;
    static int horizontallyMovingPlatformCount;
    static int deathZoneCount;
    static int viewCount;

    static const double WINDOW_WIDTH;
    static const double WINDOW_HEIGHT;

    static const double TIME_RATIO;

    static const std::string CHARACTER_OBJECT_TYPE;
    static const std::string STATIC_PLATFORM_OBJECT_TYPE;
    static const std::string VERTICALLY_MOVING_PLATFORM_OBJECT_TYPE;
    static const std::string HORIZONTALLY_MOVING_PLATFORM_OBJECT_TYPE;
    static const std::string DEATH_ZONE_OBJECT_TYPE;
    static const std::string VIEW_OBJECT_TYPE;

    explicit GameObject(std::string objectType,
                        RenderableObject *intialRenderObject = NULL,
                        MovableObject *initialMoveableObject = NULL,
                        NetworkObject *networkObject = NULL);
    ~GameObject();

    std::string getType();
    int getIndex();
    std::string getHeader();
    RenderableObject *getRenderableObject();
    MovableObject *getMovableObject();
    NetworkObject *getNetworkObject();

    void setType(std::string str);
    void setIndex(int newIndex);
    void setHeader(std::string newHeader);
    void setRenderableObject(RenderableObject *newRenderObject);
    void setMovableObject(MovableObject *newMovableObject);
    void setNetworkObject(NetworkObject *newNetworkObject);

    /**
     * This function will make this class instance accessible to scripts in
     * the given context.
     *
     * IMPORTANT: Please read this definition of this function in
     * GameObject.cpp. The helper function I've provided expects certain
     * parameters which you must use in order to take advance of this
     * convinience.
     */
    v8::Local<v8::Object> exposeToV8(v8::Isolate *isolate, v8::Local<v8::Context> &context, std::string context_name = "default");

    static void setGameObjectGUID(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info);
    static void getGameObjectGUID(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info); // note return type
    static void setGameObjectX(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info);
    static void getGameObjectX(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info);
    static void setGameObjectY(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info);
    static void getGameObjectY(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info);

private:
    std::string type;
    int index;
    std::string header;

    RenderableObject *renderObject;
    MovableObject *movableObject;
    NetworkObject *networkObject;

    v8::Isolate *isolate;
    v8::Global<v8::Context> *context;
};

#endif