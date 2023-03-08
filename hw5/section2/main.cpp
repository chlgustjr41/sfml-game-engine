#include "ScriptManager.h"
#include <v8.h>
#include <libplatform/libplatform.h>
#include <iostream>
#include "v8helpers.h"
#include <cstdio>
#include "GameObject.h"
#include "CharacterObject.h"
#include "Timeline.h"
#include "EventManager.h"
#include "EventObject.h"
#include "MovableObject.h"
#include "ProjectileObject.h"

const float WINDOW_WIDTH = 800.0;
const float WINDOW_HEIGHT = 600.0;
const float CHARACTER_SIZE = 75.0;
const float ENEMY_SIZE = 50.0;
const float PROJECTILE_SIZE = 15.0;
const float PLATFORM_HEIGHT = 50.0;
const float MOVING_PLATFORM_WIDTH = 100.0;

const long TICK_SIZE = 1;
// const std::vector<sf::Color> CHARACTER_COLORS = {sf::Color::Red, sf::Color::Blue, sf::Color::Green, sf::Color::Yellow, sf::Color::Magenta};
const std::vector<sf::Color> CHARACTER_COLORS = {sf::Color::Green, sf::Color::Yellow};

const std::vector<sf::Color> PROJECTILE_COLORS = {sf::Color::Blue, sf::Color::Red};

void projectileFactory(std::vector<GameObject *> *actors, v8::Isolate *isolate, v8::Local<v8::Context> object_context);
void projectileCollisionEventHandler(EventManager *eventManager);
void projectileClearEventHandler(EventManager *eventManager);
void deathEventHandler(EventManager *eventManager);

int main(int argc, char **argv)
{
	/*
	 * NOTE: You must initialize v8 in main, otherwise handles will go out of scope.
	 *
	 * Contexts are generally on a per-thread basis, so if you would like to do
	 * script execution in a different thread, you must create a new context in
	 * the other thread and give care to ensure the context doesn't go out of scope
	 * before the thread's run function is called.
	 *
	 * The below v8 function calls are basically boilerplate.
	 */
	std::unique_ptr<v8::Platform> platform = v8::platform::NewDefaultPlatform();
	v8::V8::InitializePlatform(platform.release());
	v8::V8::InitializeICU();
	v8::V8::Initialize();
	v8::Isolate::CreateParams create_params;
	create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
	v8::Isolate *isolate = v8::Isolate::New(create_params);

	sf::RenderWindow *window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Game");
	window->setFramerateLimit(60);

	Timeline *timeline = new Timeline(TICK_SIZE);
	std::vector<GameObject *> actors;

	// Character
	sf::RectangleShape *characterShape = new sf::RectangleShape(sf::Vector2f(CHARACTER_SIZE, CHARACTER_SIZE));
	characterShape->setFillColor(CharacterObject::CHARACTER_COLORS[0]);
	sf::Vector2f spawningPoint = sf::Vector2f((WINDOW_WIDTH - CHARACTER_SIZE) / 2, WINDOW_HEIGHT - CHARACTER_SIZE * 3 / 2);
	characterShape->setPosition(spawningPoint.x, spawningPoint.y);
	CharacterObject *character = new CharacterObject(GameObject::eventManager,
													 GameObject::CHARACTER_OBJECT_TYPE,
													 characterShape,
													 spawningPoint);

	// Enemies
	sf::Vector2f enemySpawningPoint = sf::Vector2f((WINDOW_WIDTH - CHARACTER_SIZE) / 2, CHARACTER_SIZE * 3 / 2);

	sf::RectangleShape *enemyShape0 = new sf::RectangleShape(sf::Vector2f(ENEMY_SIZE, ENEMY_SIZE));
	enemyShape0->setFillColor(CharacterObject::CHARACTER_COLORS[1]);
	enemyShape0->setPosition(enemySpawningPoint.x - 150, enemySpawningPoint.y);
	CharacterObject *enemy0 = new CharacterObject(GameObject::eventManager,
												  GameObject::ENEMY_OBJECT_TYPE,
												  enemyShape0,
												  enemySpawningPoint);
	sf::RectangleShape *enemyShape1 = new sf::RectangleShape(sf::Vector2f(ENEMY_SIZE, ENEMY_SIZE));
	enemyShape1->setFillColor(CharacterObject::CHARACTER_COLORS[1]);
	enemyShape1->setPosition(enemySpawningPoint.x, enemySpawningPoint.y);
	CharacterObject *enemy1 = new CharacterObject(GameObject::eventManager,
												  GameObject::ENEMY_OBJECT_TYPE,
												  enemyShape1,
												  enemySpawningPoint);
	sf::RectangleShape *enemyShape2 = new sf::RectangleShape(sf::Vector2f(ENEMY_SIZE, ENEMY_SIZE));
	enemyShape2->setFillColor(CharacterObject::CHARACTER_COLORS[1]);
	enemyShape2->setPosition(enemySpawningPoint.x + 150, enemySpawningPoint.y);
	CharacterObject *enemy2 = new CharacterObject(GameObject::eventManager,
												  GameObject::ENEMY_OBJECT_TYPE,
												  enemyShape2,
												  enemySpawningPoint);

	// Global Actor List
	actors.push_back(character);
	actors.push_back(enemy0);
	actors.push_back(enemy1);
	actors.push_back(enemy2);

	{											   // anonymous scope for managing handle scope
		v8::Isolate::Scope isolate_scope(isolate); // must enter the virtual machine to do stuff
		v8::HandleScope handle_scope(isolate);

		// Best practice to isntall all global functions in the context ahead of time.
		v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
		// Bind the global 'print' function to the C++ Print callback.
		global->Set(isolate, "print", v8::FunctionTemplate::New(isolate, v8helpers::Print));
		// Bind the global static function for retrieving object handles
		global->Set(isolate, "gethandle", v8::FunctionTemplate::New(isolate, ScriptManager::getHandleFromScript));

		global->Set(isolate, "getCharacterCount", v8::FunctionTemplate::New(isolate, GameObject::GetCharacterCount));
		global->Set(isolate, "getEnemyCount", v8::FunctionTemplate::New(isolate, GameObject::GetEnemyCount));
		global->Set(isolate, "getProjectileCount", v8::FunctionTemplate::New(isolate, GameObject::GetProjectileCount));

		v8::Local<v8::Context> default_context = v8::Context::New(isolate, NULL, global);
		v8::Context::Scope default_context_scope(default_context); // enter the context

		ScriptManager *sm = new ScriptManager(isolate, default_context);

		// Create a new context
		v8::Local<v8::Context> object_context = v8::Context::New(isolate, NULL, global);
		sm->addContext(isolate, object_context, "object_context");

		sm->addScript("character_movement", "scripts/character_movement.js", "object_context");
		sm->addScript("enemy_movement", "scripts/enemy_movement.js", "object_context");

		character->exposeToV8(isolate, object_context, "object_context");
		enemy0->exposeToV8(isolate, object_context, "object_context");
		enemy1->exposeToV8(isolate, object_context, "object_context");
		enemy2->exposeToV8(isolate, object_context, "object_context");

		bool isRunning = true;
		while (isRunning)
		{
			double duration = timeline->getDuration();
			character->updateDuration(duration);

			sf::Event event;
			while (window->pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					window->close();
					isRunning = false;
				}
			}

			// Character input
			std::string inputs = "";
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				inputs += "Left,";
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				inputs += "Right,";
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
				inputs += "Space,";
			}
			GameObject::eventManager->registerEvent(new EventObject(EventObject::CHARACTER_INPUT,
																	EventManager::CHARACTER_INPUT_PRIORITY,
																	character->getHeader(),
																	inputs));

			// Trigger event listeners
			character->checkInputEvent();
			projectileFactory(&actors, isolate, object_context);

			projectileCollisionEventHandler(GameObject::eventManager);
			projectileClearEventHandler(GameObject::eventManager);
			deathEventHandler(GameObject::eventManager);

			// Trigger scripts
			sm->runOne("character_movement", false, "object_context");
			sm->runOne("enemy_movement", false, "object_context");

			window->clear(sf::Color::Black);

			// deathZone->render(window);
			// view->render(window);

			character->render(window);
			for (int i = 0; i < GameObject::projectileObjects.size(); i++)
			{
				static_cast<ProjectileObject *>(GameObject::projectileObjects[i])->render(window);
			}
			for (int i = 0; i < GameObject::enemyObjects.size(); i++)
			{
				static_cast<CharacterObject *>(GameObject::enemyObjects[i])->render(window);
			}

			window->display();
		}
	}

	isolate->Dispose();
	v8::V8::Dispose();
	v8::V8::ShutdownPlatform();

	return 0;
}

void projectileFactory(std::vector<GameObject *> *actors, v8::Isolate *isolate, v8::Local<v8::Context> object_context)
{
	for (int i = 0; i < actors->size(); i++)
	{
		std::string sourceHeader = (*actors)[i]->getHeader();
		EventObject *currentEvent = GameObject::eventManager->pollEvent(EventObject::PROJECTILE_LAUNCH, sourceHeader);

		if (currentEvent == NULL)
			return;

		sf::Vector2f sourcePos;
		for (int itr = 0; itr < actors->size(); itr++)
		{
			sourcePos = (*actors)[i]->getObjectPosition();
		}

		if (currentEvent != NULL && sourceHeader != "")
		{
			ProjectileObject *newLaunch;
			sf::RectangleShape *shape = new sf::RectangleShape(sf::Vector2f(PROJECTILE_SIZE, PROJECTILE_SIZE));
			if (sourceHeader.find(GameObject::CHARACTER_OBJECT_TYPE) != std::string::npos)
			{
				sourcePos.x += (CHARACTER_SIZE - PROJECTILE_SIZE) / 2;
				shape->setPosition(sourcePos);
				shape->setFillColor(PROJECTILE_COLORS[0]);
				newLaunch = new ProjectileObject(GameObject::eventManager,
												 shape,
												 &GameObject::enemyObjects,
												 sourceHeader);
			}
			else if (sourceHeader.find(GameObject::ENEMY_OBJECT_TYPE) != std::string::npos)
			{
				sourcePos.x += (CHARACTER_SIZE - PROJECTILE_SIZE) / 2;
				shape->setPosition(sourcePos);
				shape->setFillColor(PROJECTILE_COLORS[1]);
				newLaunch = new ProjectileObject(GameObject::eventManager,
												 shape,
												 &GameObject::characterObjects,
												 sourceHeader);
			}
			else
			{
				return;
			}

			newLaunch->exposeToV8(isolate, object_context, "object_context");
		}
	}
}

void projectileCollisionEventHandler(EventManager *eventManager)
{
	for (int i = GameObject::projectileObjects.size() - 1; i >= 0; i--)
	{
		auto projectile = GameObject::projectileObjects[i]->getMovableObject()->getMovingObject();
		if (projectile->getPosition().x < 0 || projectile->getPosition().x > WINDOW_WIDTH ||
			projectile->getPosition().y < 0 || projectile->getPosition().y > WINDOW_HEIGHT)
		{
			eventManager->registerEvent(new EventObject(EventObject::PROJECTILE_CLEAR,
														EventManager::PROJECTILE_CLEAR_PRIORITY,
														GameObject::projectileObjects[i]->getHeader(),
														""));
		}
	}
	for (int i = 0; i < GameObject::projectileObjects.size(); i++)
	{
		static_cast<ProjectileObject *>(GameObject::projectileObjects[i])->checkEnemyCollision();
	}
}

void projectileClearEventHandler(EventManager *eventManager)
{
	EventObject *currentEvent;
	currentEvent = eventManager->pollEvent(EventObject::PROJECTILE_CLEAR, GameObject::CHARACTER_OBJECT_TYPE);
	if (currentEvent == NULL)
	{
		for (int i = GameObject::projectileObjects.size() - 1; i >= 0; i--)
		{
			auto projectile = GameObject::projectileObjects[i]->getMovableObject()->getMovingObject();
			if (projectile->getPosition().x < 0 || projectile->getPosition().x > WINDOW_WIDTH ||
				projectile->getPosition().y < 0 || projectile->getPosition().y > WINDOW_HEIGHT)
			{
				GameObject::projectileObjects.erase(GameObject::projectileObjects.begin() + i);
				GameObject::projectileCount -= 1;
			}
		}
	}
}

void deathEventHandler(EventManager *eventManager)
{
	EventObject *currentEvent;
	if (GameObject::characterObjects.size() > 0)
	{
		for (int i = GameObject::characterObjects.size() - 1; i >= 0; i--)
		{
			currentEvent = eventManager->pollEvent(EventObject::CHARACTER_DEATH, GameObject::characterObjects[i]->getHeader());
			if (currentEvent)
			{
				GameObject::characterObjects.erase(GameObject::characterObjects.begin() + i);
				GameObject::characterCount -= 1;
			}
		}
	}
	if (GameObject::enemyObjects.size() > 0)
	{
		for (int i = GameObject::enemyObjects.size() - 1; i >= 0; i--)
		{
			currentEvent = eventManager->pollEvent(EventObject::CHARACTER_DEATH, GameObject::enemyObjects[i]->getHeader());
			if (currentEvent)
			{
				GameObject::enemyObjects.erase(GameObject::enemyObjects.begin() + i);
				GameObject::enemyCount -= 1;
			}
		}
	}
}