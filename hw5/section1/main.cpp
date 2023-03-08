#include "ScriptManager.h"
#include <v8.h>
#include <libplatform/libplatform.h>
#include <iostream>
#include "v8helpers.h"
#include <cstdio>
#include "GameObject.h"
#include "CharacterObject.h"
#include "Timeline.h"
#include "StaticPlatformObject.h"
#include "VerticallyMovingPlatformObject.h"
#include "HorizontallyMovingPlatformObject.h"
#include "DeathZoneObject.h"
#include "ViewObject.h"
#include "EventManager.h"
#include "EventObject.h"

const float WINDOW_WIDTH = 800.0;
const float WINDOW_HEIGHT = 600.0;
const float CHARACTER_SIZE = 75.0;
const float PLATFORM_HEIGHT = 50.0;
const float MOVING_PLATFORM_WIDTH = 100.0;

const long TICK_SIZE = 1;
const std::vector<sf::Color> CHARACTER_COLORS = {sf::Color::Red, sf::Color::Blue, sf::Color::Green, sf::Color::Yellow, sf::Color::Magenta};

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

	// EventManager *eventManager = new EventManager();

	// Static platform 1
	sf::RectangleShape *staticPlatformShape1 = new sf::RectangleShape(sf::Vector2f(WINDOW_WIDTH, PLATFORM_HEIGHT));
	staticPlatformShape1->setPosition(0, WINDOW_HEIGHT - PLATFORM_HEIGHT);
	StaticPlatformObject *staticPlatform1 = new StaticPlatformObject(staticPlatformShape1);
	// Static platform 2
	sf::RectangleShape *staticPlatformShape2 = new sf::RectangleShape(sf::Vector2f(WINDOW_WIDTH, PLATFORM_HEIGHT));
	staticPlatformShape2->setPosition(WINDOW_WIDTH + CHARACTER_SIZE + 100, WINDOW_HEIGHT - PLATFORM_HEIGHT);
	StaticPlatformObject *staticPlatform2 = new StaticPlatformObject(staticPlatformShape2);
	// Vertically moving platform 1
	sf::RectangleShape *verticallyMovingPlatformShape1 = new sf::RectangleShape(sf::Vector2f(MOVING_PLATFORM_WIDTH, PLATFORM_HEIGHT));
	verticallyMovingPlatformShape1->setPosition(300, WINDOW_HEIGHT / 2);
	VerticallyMovingPlatformObject *verticallyMovingPlatform1 = new VerticallyMovingPlatformObject(GameObject::eventManager, verticallyMovingPlatformShape1, 400);
	// Vertically moving platform 2
	sf::RectangleShape *verticallyMovingPlatformShape2 = new sf::RectangleShape(sf::Vector2f(MOVING_PLATFORM_WIDTH, PLATFORM_HEIGHT));
	verticallyMovingPlatformShape2->setPosition(1100, WINDOW_HEIGHT / 2);
	VerticallyMovingPlatformObject *verticallyMovingPlatform2 = new VerticallyMovingPlatformObject(GameObject::eventManager, verticallyMovingPlatformShape2, 400);
	// Horizontally moving platform 1
	sf::RectangleShape *horizontallyMovingPlatformShape1 = new sf::RectangleShape(sf::Vector2f(MOVING_PLATFORM_WIDTH, PLATFORM_HEIGHT));
	horizontallyMovingPlatformShape1->setPosition(850, 100);
	HorizontallyMovingPlatformObject *horizontallyMovingPlatform1 = new HorizontallyMovingPlatformObject(GameObject::eventManager, horizontallyMovingPlatformShape1, 300);
	// Platforms
	std::vector<GameObject *> platforms;
	platforms.push_back(staticPlatform1);
	platforms.push_back(staticPlatform2);
	platforms.push_back(verticallyMovingPlatform1);
	platforms.push_back(verticallyMovingPlatform2);
	platforms.push_back(horizontallyMovingPlatform1);

	// Character
	sf::RectangleShape *characterBoundary = new sf::RectangleShape(sf::Vector2f(WINDOW_WIDTH * 4, WINDOW_HEIGHT));
	characterBoundary->setPosition(-WINDOW_WIDTH, 0);
	sf::RectangleShape *characterShape = new sf::RectangleShape(sf::Vector2f(CHARACTER_SIZE, CHARACTER_SIZE));
	characterShape->setPosition(100, (WINDOW_HEIGHT - CHARACTER_SIZE) / 2);
	CharacterObject *character = new CharacterObject(GameObject::eventManager,
													 characterShape,
													 characterBoundary,
													 CharacterObject::CHARACTER_COLORS[GameObject::characterCount % CharacterObject::CHARACTER_COLORS.size()],
													 &platforms); // GameObject::collidableObjects
	// Characters
	std::vector<CharacterObject *> characters;
	characters.push_back(character);

	// View adjuster for the character
	sf::RectangleShape *viewBoundary = new sf::RectangleShape(sf::Vector2f(CHARACTER_SIZE * 2, CHARACTER_SIZE * 2));
	viewBoundary->setPosition(characterShape->getPosition().x - (viewBoundary->getSize().x - characterShape->getSize().x) / 2,
							  characterShape->getPosition().y - (viewBoundary->getSize().y - characterShape->getSize().y) / 2);
	viewBoundary->setFillColor(sf::Color::Blue);
	ViewObject *view = new ViewObject(GameObject::eventManager, viewBoundary, characterShape, 1);
	// Set default starting view
	sf::View windowView = window->getView();
	windowView.setCenter(characterShape->getPosition().x, windowView.getCenter().y);
	window->setView(windowView);

	// Spawn/Death Zone
	sf::RectangleShape *spawnZoneShape = new sf::RectangleShape(sf::Vector2f(CHARACTER_SIZE, CHARACTER_SIZE));
	spawnZoneShape->setPosition(100, WINDOW_HEIGHT / 2 - 200);
	spawnZoneShape->setFillColor(sf::Color::Green);
	sf::RectangleShape *deathZoneShape = new sf::RectangleShape(sf::Vector2f(WINDOW_WIDTH * 5, PLATFORM_HEIGHT));
	deathZoneShape->setPosition(-WINDOW_WIDTH, WINDOW_HEIGHT + PLATFORM_HEIGHT);
	deathZoneShape->setFillColor(sf::Color::Red);
	DeathZoneObject *deathZone = new DeathZoneObject(GameObject::eventManager, deathZoneShape, spawnZoneShape, &characters);

	{											   // anonymous scope for managing handle scope
		v8::Isolate::Scope isolate_scope(isolate); // must enter the virtual machine to do stuff
		v8::HandleScope handle_scope(isolate);

		// Best practice to isntall all global functions in the context ahead of time.
		v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
		// Bind the global 'print' function to the C++ Print callback.
		global->Set(isolate, "print", v8::FunctionTemplate::New(isolate, v8helpers::Print));
		// Bind the global static factory function for creating new GameObject instances
		global->Set(isolate, "spawnCharacter", v8::FunctionTemplate::New(isolate, CharacterObject::SpawnCharacter));

		v8::Local<v8::Context> default_context = v8::Context::New(isolate, NULL, global);
		v8::Context::Scope default_context_scope(default_context); // enter the context

		ScriptManager *sm = new ScriptManager(isolate, default_context);

		// Create a new context
		v8::Local<v8::Context> object_context = v8::Context::New(isolate, NULL, global);
		sm->addContext(isolate, object_context, "object_context");

		sm->addScript("character_movement", "scripts/character_movement.js", "object_context");

		character->exposeToV8(isolate, object_context, "object_context");

		bool isRunning = true;
		while (isRunning)
		{
			double duration = timeline->getDuration();
			view->updateView(window);
			character->updateDuration(duration);
			verticallyMovingPlatform1->updateDuration(duration);
			verticallyMovingPlatform2->updateDuration(duration);
			horizontallyMovingPlatform1->updateDuration(duration);

			character->run();
			verticallyMovingPlatform1->run();
			verticallyMovingPlatform2->run();
			horizontallyMovingPlatform1->run();

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
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) &&
				!character->getIsJumping())
			{
				inputs += "Space,";
			}
			GameObject::eventManager->registerEvent(new EventObject(EventObject::CHARACTER_INPUT,
																	EventManager::CHARACTER_INPUT_PRIORITY,
																	character->getHeader(),
																	inputs));

			// Trigger event listeners
			character->checkInputEvent();
			character->checkPlatformCollideEvent();
			deathZone->checkDeathEvent();
			deathZone->checkSpawnEvent();

			// Trigger scripts
			sm->runOne("character_movement", false, "object_context");

			window->clear(sf::Color::Black);

			// deathZone->render(window);
			// view->render(window);

			character->render(window);
			staticPlatform1->render(window);
			staticPlatform2->render(window);
			verticallyMovingPlatform1->render(window);
			verticallyMovingPlatform2->render(window);
			horizontallyMovingPlatform1->render(window);

			window->display();
		}
	}

	isolate->Dispose();
	v8::V8::Dispose();
	v8::V8::ShutdownPlatform();

	return 0;
}
