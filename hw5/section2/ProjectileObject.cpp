
#include "ProjectileObject.h"

ProjectileObject::ProjectileObject(EventManager *initialEventManager,
                                   sf::RectangleShape *initialObjectShape,
                                   std::vector<GameObject *> *initialCharacters,
                                   std::string initialSourceType)
    : GameObject(GameObject::PROJECTILE_OBJECT_TYPE,
                 new RenderableObject(initialObjectShape),
                 new ProjectileMovableObject(initialEventManager,
                                             GameObject::PROJECTILE_OBJECT_TYPE,
                                             GameObject::projectileCount,
                                             initialObjectShape,
                                             initialCharacters))
{
    eventManager = initialEventManager;
    characters = initialCharacters;
    sourceType = initialSourceType;
}

void ProjectileObject::checkEnemyCollision()
{
    for (int i = 0; i < characters->size(); i++)
    {
        if ((*characters)[i]->getHeader().find(sourceType) == std::string::npos &&
            GameObject::getRenderableObject()->getObjectShape()->getGlobalBounds().intersects((*characters)[i]->getRenderableObject()->getObjectShape()->getGlobalBounds()))
        {
            eventManager->registerEvent(new EventObject(EventObject::CHARACTER_DEATH,
                                                        EventManager::CHARACTER_DEATH_PRIORITY,
                                                        (*characters)[i]->getHeader(),
                                                        ""));
            eventManager->registerEvent(new EventObject(EventObject::PROJECTILE_CLEAR,
                                                        EventManager::PROJECTILE_CLEAR_PRIORITY,
                                                        getHeader(),
                                                        ""));
        }
    }
}

void ProjectileObject::render(sf::RenderWindow *window)
{
    GameObject::getRenderableObject()->render(window);
}

std::string ProjectileObject::getSourceType()
{
    return sourceType;
}
void ProjectileObject::setSourceType(std::string newSourceType)
{
    sourceType = newSourceType;
}

v8::Local<v8::Object> ProjectileObject::exposeToV8(v8::Isolate *isolate, v8::Local<v8::Context> &context, std::string context_name)
{
    // GameObject::exposeToV8(isolate, context, context_name);
    std::vector<v8helpers::ParamContainer<v8::AccessorGetterCallback, v8::AccessorSetterCallback>> v;
    v.push_back(v8helpers::ParamContainer("sourceType", getGameObjectSourceType, setGameObjectSourceType));
    v.push_back(v8helpers::ParamContainer("guid", getGameObjectGUID, setGameObjectGUID));
    v.push_back(v8helpers::ParamContainer("x", getGameObjectX, setGameObjectX));
    v.push_back(v8helpers::ParamContainer("y", getGameObjectY, setGameObjectY));
    return v8helpers::exposeToV8(getHeader(), this, v, isolate, context, context_name);
}

void ProjectileObject::setGameObjectSourceType(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info)
{
    v8::Local<v8::Object> self = info.Holder();
    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
    void *ptr = wrap->Value();

    v8::String::Utf8Value utf8_str(info.GetIsolate(), value->ToString());
    static_cast<ProjectileObject *>(ptr)->setSourceType(*utf8_str);
}
void ProjectileObject::getGameObjectSourceType(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info)
{
    v8::Local<v8::Object> self = info.Holder();
    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
    void *ptr = wrap->Value();
    std::string sourceType = static_cast<ProjectileObject *>(ptr)->getSourceType();
    v8::Local<v8::String> v8_sourceType = v8::String::NewFromUtf8(info.GetIsolate(), sourceType.c_str(), v8::String::kNormalString);
    info.GetReturnValue().Set(v8_sourceType);
}