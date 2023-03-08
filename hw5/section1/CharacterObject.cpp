#include "CharacterObject.h"

const std::vector<sf::Color> CharacterObject::CHARACTER_COLORS =
    {sf::Color::Red, sf::Color::Blue, sf::Color::Green, sf::Color::Yellow, sf::Color::Magenta};
const float CharacterObject::CHARACTER_SIZE = 75.0;

CharacterObject::CharacterObject(EventManager *eventManager,
                                 sf::RectangleShape *initialObjectShape,
                                 sf::RectangleShape *boundary,
                                 sf::Color initialColor,
                                 std::vector<GameObject *> *initialCollidableObjects)
    : GameObject(GameObject::CHARACTER_OBJECT_TYPE,
                 new RenderableObject(initialObjectShape),
                 new CharacterMovableObject(eventManager,
                                            GameObject::CHARACTER_OBJECT_TYPE,
                                            GameObject::characterCount,
                                            initialObjectShape,
                                            boundary,
                                            initialCollidableObjects),
                 new CharacterNetworkObject(GameObject::CHARACTER_OBJECT_TYPE,
                                            GameObject::characterCount,
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

bool CharacterObject::getIsJumping()
{
    return static_cast<CharacterMovableObject *>(GameObject::getMovableObject())->getIsJumping();
}

void CharacterObject::SpawnCharacter(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    v8::Isolate *isolate = args.GetIsolate();
    v8::Local<v8::Context> context = isolate->GetCurrentContext();
    v8::EscapableHandleScope handle_scope(args.GetIsolate());
    v8::Context::Scope context_scope(context);

    double x = 0;
    double y = 0;
    if (args.Length() == 2)
    {
        v8::String::Utf8Value xPos(args.GetIsolate(), args[0]);
        v8::String::Utf8Value yPos(args.GetIsolate(), args[1]);
        x = std::stod(std::string(v8helpers::ToCString(xPos)));
        y = std::stod(std::string(v8helpers::ToCString(yPos)));
    }

    sf::RectangleShape *characterShape = new sf::RectangleShape(sf::Vector2f(CharacterObject::CHARACTER_SIZE,
                                                                             CharacterObject::CHARACTER_SIZE));
    sf::RectangleShape *characterBoundary = new sf::RectangleShape(sf::Vector2f(GameObject::WINDOW_WIDTH * 4,
                                                                                GameObject::WINDOW_HEIGHT));
    characterShape->setPosition(x, y);
    characterBoundary->setPosition(characterShape->getPosition().x - 2 * GameObject::WINDOW_WIDTH + CharacterObject::CHARACTER_SIZE / 2,
                                   characterShape->getPosition().y);

    GameObject *new_object = new CharacterObject(GameObject::eventManager,
                                                 characterShape,    // shape
                                                 characterBoundary, // boundary
                                                 CharacterObject::CHARACTER_COLORS[GameObject::characterCount % CharacterObject::CHARACTER_COLORS.size()],
                                                 GameObject::collidableObjects);
    v8::Local<v8::Object> v8_obj = new_object->exposeToV8(isolate, context);
    args.GetReturnValue().Set(handle_scope.Escape(v8_obj));
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
    static_cast<CharacterMovableObject *>(movingObject)->setScriptIsMovingLeft(value->NumberValue());
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
    static_cast<CharacterMovableObject *>(movingObject)->setScriptIsMovingLeft(value->BooleanValue());
}
void CharacterObject::getGameObjectIsMovingLeft(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info)
{
    v8::Local<v8::Object> self = info.Holder();
    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
    void *ptr = wrap->Value();

    auto movingObject = static_cast<GameObject *>(ptr)->getMovableObject();
    bool isMovingLeft = static_cast<CharacterMovableObject *>(movingObject)->getScriptIsMovingLeft();
    info.GetReturnValue().Set(isMovingLeft);
}
void CharacterObject::setGameObjectIsMovingRight(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info)
{
    v8::Local<v8::Object> self = info.Holder();
    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
    void *ptr = wrap->Value();

    auto movingObject = static_cast<GameObject *>(ptr)->getMovableObject();
    static_cast<CharacterMovableObject *>(movingObject)->setScriptIsMovingRight(value->BooleanValue());
}
void CharacterObject::getGameObjectIsMovingRight(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info)
{
    v8::Local<v8::Object> self = info.Holder();
    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
    void *ptr = wrap->Value();

    auto movingObject = static_cast<GameObject *>(ptr)->getMovableObject();
    bool isMovingRight = static_cast<CharacterMovableObject *>(movingObject)->getScriptIsMovingRight();
    info.GetReturnValue().Set(isMovingRight);
}
