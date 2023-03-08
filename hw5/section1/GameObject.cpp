#include <filesystem>
#include <cstring>
#include <sstream>
#include "v8helpers.h"
#include "GameObject.h"

EventManager *GameObject::eventManager = new EventManager();
std::vector<GameObject *> *GameObject::collidableObjects;
std::vector<GameObject *> *GameObject::characterObjects;

int GameObject::characterCount = 0;
int GameObject::staticPlatformCount = 0;
int GameObject::verticallyMovingPlatformCount = 0;
int GameObject::horizontallyMovingPlatformCount = 0;
int GameObject::deathZoneCount = 0;
int GameObject::viewCount = 0;

const double GameObject::WINDOW_WIDTH = 800.0;
const double GameObject::WINDOW_HEIGHT = 600.0;

const double GameObject::TIME_RATIO = 0.5;

const std::string GameObject::CHARACTER_OBJECT_TYPE = "character";
const std::string GameObject::STATIC_PLATFORM_OBJECT_TYPE = "staticPlatform";
const std::string GameObject::VERTICALLY_MOVING_PLATFORM_OBJECT_TYPE = "verticallyMovingPlatform";
const std::string GameObject::HORIZONTALLY_MOVING_PLATFORM_OBJECT_TYPE = "horizontallyMovingPlatform";
const std::string GameObject::DEATH_ZONE_OBJECT_TYPE = "deathZone";
const std::string GameObject::VIEW_OBJECT_TYPE = "view";

GameObject::GameObject(std::string objectType,
                       RenderableObject *intialRenderObject,
                       MovableObject *initialMoveableObject,
                       NetworkObject *initialNetworkObject)
{
    type = objectType;
    renderObject = intialRenderObject;
    movableObject = initialMoveableObject;
    networkObject = initialNetworkObject;

    if (objectType == CHARACTER_OBJECT_TYPE)
    {
        index = characterCount;
        characterCount++;
    }
    else if (objectType == STATIC_PLATFORM_OBJECT_TYPE)
    {
        index = staticPlatformCount;
        ++staticPlatformCount;
    }
    else if (objectType == VERTICALLY_MOVING_PLATFORM_OBJECT_TYPE)
    {
        index = verticallyMovingPlatformCount;
        ++verticallyMovingPlatformCount;
    }
    else if (objectType == HORIZONTALLY_MOVING_PLATFORM_OBJECT_TYPE)
    {
        index = horizontallyMovingPlatformCount;
        ++horizontallyMovingPlatformCount;
    }
    else if (objectType == DEATH_ZONE_OBJECT_TYPE)
    {
        index = deathZoneCount;
        ++deathZoneCount;
    }
    else if (objectType == VIEW_OBJECT_TYPE)
    {
        index = viewCount;
        ++viewCount;
    }

    header = type + std::to_string(index);
}
GameObject::~GameObject()
{
    context->Reset();
}

std::string GameObject::getType()
{
    return type;
}
int GameObject::getIndex()
{
    return index;
}
std::string GameObject::getHeader()
{
    return header;
}
RenderableObject *GameObject::getRenderableObject()
{
    return renderObject;
}
MovableObject *GameObject::getMovableObject()
{
    return movableObject;
}
NetworkObject *GameObject::getNetworkObject()
{
    return networkObject;
}

void GameObject::setType(std::string str)
{
    type = str;
    setHeader(type + std::to_string(index));
}
void GameObject::setIndex(int newIndex)
{
    index = newIndex;
    setHeader(type + std::to_string(index));
}
void GameObject::setHeader(std::string newHeader)
{
    header = newHeader;

    std::string updateType;
    int updateIndex;
    std::istringstream sstream(newHeader);
    sstream >> updateType >> updateIndex;
    setType(updateType);
    setIndex(updateIndex);
}
void GameObject::setRenderableObject(RenderableObject *newRenderObject)
{
    renderObject = newRenderObject;
}
void GameObject::setMovableObject(MovableObject *newMovableObject)
{
    movableObject = newMovableObject;
}
void GameObject::setNetworkObject(NetworkObject *newNetworkObject)
{
    networkObject = newNetworkObject;
}

v8::Local<v8::Object> GameObject::exposeToV8(v8::Isolate *isolate, v8::Local<v8::Context> &context, std::string context_name)
{
    std::vector<v8helpers::ParamContainer<v8::AccessorGetterCallback, v8::AccessorSetterCallback>> v;
    v.push_back(v8helpers::ParamContainer("guid", getGameObjectGUID, setGameObjectGUID));
    v.push_back(v8helpers::ParamContainer("x", getGameObjectX, setGameObjectX));
    v.push_back(v8helpers::ParamContainer("y", getGameObjectY, setGameObjectY));
    return v8helpers::exposeToV8(header, this, v, isolate, context, context_name);
}

void GameObject::setGameObjectGUID(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info)
{
    v8::Local<v8::Object> self = info.Holder();
    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
    void *ptr = wrap->Value();
    v8::String::Utf8Value utf8_str(info.GetIsolate(), value->ToString());
    static_cast<GameObject *>(ptr)->setHeader(*utf8_str);
}
void GameObject::getGameObjectGUID(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info)
{
    v8::Local<v8::Object> self = info.Holder();
    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
    void *ptr = wrap->Value();
    std::string guid = static_cast<GameObject *>(ptr)->getHeader();
    v8::Local<v8::String> v8_guid = v8::String::NewFromUtf8(info.GetIsolate(), guid.c_str(), v8::String::kNormalString);
    info.GetReturnValue().Set(v8_guid);
}
void GameObject::setGameObjectX(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info)
{
    v8::Local<v8::Object> self = info.Holder();
    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
    void *ptr = wrap->Value();

    auto movingObject = static_cast<GameObject *>(ptr)->getMovableObject()->getMovingObject();
    movingObject->setPosition(value->NumberValue(), movingObject->getPosition().y);
}
void GameObject::getGameObjectX(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info)
{
    v8::Local<v8::Object> self = info.Holder();
    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
    void *ptr = wrap->Value();
    double x = static_cast<GameObject *>(ptr)->getMovableObject()->getMovingObject()->getPosition().x;
    info.GetReturnValue().Set(x);
}
void GameObject::setGameObjectY(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info)
{
    v8::Local<v8::Object> self = info.Holder();
    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
    void *ptr = wrap->Value();

    auto movingObject = static_cast<GameObject *>(ptr)->getMovableObject()->getMovingObject();
    movingObject->setPosition(movingObject->getPosition().x, value->NumberValue());
}
void GameObject::getGameObjectY(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info)
{
    v8::Local<v8::Object> self = info.Holder();
    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
    void *ptr = wrap->Value();
    double y = static_cast<GameObject *>(ptr)->getMovableObject()->getMovingObject()->getPosition().y;
    info.GetReturnValue().Set(y);
}