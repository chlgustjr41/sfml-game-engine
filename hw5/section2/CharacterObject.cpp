#include "CharacterObject.h"

const std::vector<sf::Color> CharacterObject::CHARACTER_COLORS = {sf::Color::Green, sf::Color::Yellow};
const float CharacterObject::CHARACTER_SIZE = 75.0;

CharacterObject::CharacterObject(EventManager *eventManager,
                                 std::string objectType,
                                 sf::RectangleShape *initialObjectShape,
                                 sf::Vector2f objectSpawningPoint)
    : GameObject(objectType,
                 new RenderableObject(initialObjectShape),
                 new CharacterMovableObject(eventManager,
                                            GameObject::CHARACTER_OBJECT_TYPE,
                                            GameObject::characterCount,
                                            initialObjectShape,
                                            objectSpawningPoint))
{
}

std::string CharacterObject::getHeader()
{
    return GameObject::getHeader();
}
RenderableObject *CharacterObject::getRenderableObject()
{
    return GameObject::getRenderableObject();
}
MovableObject *CharacterObject::getMovableObject()
{
    return GameObject::getMovableObject();
}
NetworkObject *CharacterObject::getNetworkObject()
{
    return GameObject::getNetworkObject();
}
sf::Vector2f CharacterObject::getObjectPosition()
{
    return GameObject::getObjectPosition();
}

void CharacterObject::checkInputEvent()
{
    static_cast<CharacterMovableObject *>(GameObject::getMovableObject())->checkInputEvent();
}

void CharacterObject::updateDuration(double duration)
{
    static_cast<CharacterMovableObject *>(GameObject::getMovableObject())->setDuration(duration);
}

void CharacterObject::render(sf::RenderWindow *window)
{
    GameObject::getRenderableObject()->render(window);
}

void CharacterObject::run()
{
    static_cast<CharacterMovableObject *>(GameObject::getMovableObject())->run();
}

v8::Local<v8::Object> CharacterObject::exposeToV8(v8::Isolate *isolate, v8::Local<v8::Context> &context, std::string context_name)
{
    // GameObject::exposeToV8(isolate, context, context_name);
    std::vector<v8helpers::ParamContainer<v8::AccessorGetterCallback, v8::AccessorSetterCallback>> v;
    v.push_back(v8helpers::ParamContainer("duration", getGameObjectDuration, setGameObjectDuration));
    v.push_back(v8helpers::ParamContainer("isMovingLeft", getGameObjectIsMovingLeft, setGameObjectIsMovingLeft));
    v.push_back(v8helpers::ParamContainer("isMovingRight", getGameObjectIsMovingRight, setGameObjectIsMovingRight));
    v.push_back(v8helpers::ParamContainer("guid", getGameObjectGUID, setGameObjectGUID));
    v.push_back(v8helpers::ParamContainer("x", getGameObjectX, setGameObjectX));
    v.push_back(v8helpers::ParamContainer("y", getGameObjectY, setGameObjectY));
    return v8helpers::exposeToV8(getHeader(), this, v, isolate, context, context_name);
}

void CharacterObject::setGameObjectDuration(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info)
{
    v8::Local<v8::Object> self = info.Holder();
    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
    void *ptr = wrap->Value();

    auto movingObject = static_cast<GameObject *>(ptr)->getMovableObject();
    static_cast<CharacterMovableObject *>(movingObject)->setDuration(value->NumberValue());
}
void CharacterObject::getGameObjectDuration(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info)
{
    v8::Local<v8::Object> self = info.Holder();
    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
    void *ptr = wrap->Value();

    auto movingObject = static_cast<GameObject *>(ptr)->getMovableObject();
    double duration = static_cast<CharacterMovableObject *>(movingObject)->getDuration();
    info.GetReturnValue().Set(duration);
}
void CharacterObject::setGameObjectIsMovingLeft(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info)
{
    v8::Local<v8::Object> self = info.Holder();
    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
    void *ptr = wrap->Value();

    auto movingObject = static_cast<GameObject *>(ptr)->getMovableObject();
    static_cast<CharacterMovableObject *>(movingObject)->setIsMovingLeft(value->BooleanValue());
}
void CharacterObject::getGameObjectIsMovingLeft(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info)
{
    v8::Local<v8::Object> self = info.Holder();
    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
    void *ptr = wrap->Value();

    auto movingObject = static_cast<GameObject *>(ptr)->getMovableObject();
    bool isMovingLeft = static_cast<CharacterMovableObject *>(movingObject)->getIsMovingLeft();
    info.GetReturnValue().Set(isMovingLeft);
}
void CharacterObject::setGameObjectIsMovingRight(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info)
{
    v8::Local<v8::Object> self = info.Holder();
    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
    void *ptr = wrap->Value();

    auto movingObject = static_cast<GameObject *>(ptr)->getMovableObject();
    static_cast<CharacterMovableObject *>(movingObject)->setIsMovingRight(value->BooleanValue());
}
void CharacterObject::getGameObjectIsMovingRight(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info)
{
    v8::Local<v8::Object> self = info.Holder();
    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
    void *ptr = wrap->Value();

    auto movingObject = static_cast<GameObject *>(ptr)->getMovableObject();
    bool isMovingRight = static_cast<CharacterMovableObject *>(movingObject)->getIsMovingRight();
    info.GetReturnValue().Set(isMovingRight);
}
