#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include <vector>
#include <chrono>
#include <thread>
#include <mutex>
#include <iostream>
#include <zmq.hpp>
#include "CharacterObject.h"
#include "Timeline.h"
#include "StaticPlatformObject.h"
#include "VerticallyMovingPlatformObject.h"
#include "HorizontallyMovingPlatformObject.h"
#include "DeathZoneObject.h"
#include "ViewObject.h"
#include "EventManager.h"
#include "EventObject.h"

std::string const CLIENT_CONNECTION_REQUEST_HEADER = "Client Connection Request";
std::string const SERVER_CONNECTION_RESPONSE_HEADER = "Server Connection Response";

// Game default constant values
const float WINDOW_WIDTH = 800.0;
const float WINDOW_HEIGHT = 600.0;
const float CHARACTER_SIZE = 75.0;
const float PLATFORM_HEIGHT = 50.0;
const float MOVING_PLATFORM_WIDTH = 100.0;
const int PLATFORM_COUNT = 2;
const std::vector<sf::Color> CHARACTER_COLORS = {sf::Color::Red, sf::Color::Blue, sf::Color::Green, sf::Color::Yellow, sf::Color::Magenta};

void subscribeClientInput(int port, std::vector<std::string> *userInputs, std::mutex *userInputsMutex)
{
    // Initializing publish subscribe
    zmq::context_t context{1};
    zmq::socket_t clientConnection{context, zmq::socket_type::sub};
    // Connecting and subscribing to player client publish socket
    int conflate = 1;
    clientConnection.setsockopt(ZMQ_CONFLATE, &conflate, sizeof(conflate));
    clientConnection.connect("tcp://localhost:" + std::to_string(port));
    clientConnection.setsockopt(ZMQ_SUBSCRIBE, "", 0);

    bool isRunning = true;
    while (isRunning)
    {
        zmq::message_t data;
        clientConnection.recv(data, zmq::recv_flags::none);

        userInputsMutex->lock();
        userInputs->push_back(data.to_string());
        userInputsMutex->unlock();
    }
}

void publishGameState(EventManager *eventManager,
                      std::vector<std::string> *userInputs,
                      std::vector<GameObject *> *platforms,
                      std::vector<CharacterObject *> *players,
                      DeathZoneObject *deathZone,
                      std::mutex *userInputsMutex,
                      std::mutex *playersMutex)
{
    using namespace std::chrono_literals;
    const double DEFAULT_TICK = 1;

    // Initializing publish socket
    zmq::context_t context{1};
    zmq::socket_t publishStateSocket{context, zmq::socket_type::pub};
    publishStateSocket.bind("tcp://*:5001");

    Timeline *timeline = new Timeline(DEFAULT_TICK);

    bool isRunning = true;
    while (isRunning)
    {
        std::this_thread::sleep_for(1ms); // Optimized thread execution time
        double duration = timeline->getDuration();

        deathZone->checkDeathEvent();
        deathZone->checkSpawnEvent();

        for (int i = 0; i < players->size(); i++)
        {
            (*players)[i]->updateDuration(duration);
            (*players)[i]->run();
        }
        for (int i = 0; i < platforms->size(); i++)
        {
            if ((*platforms)[i]->getType() == GameObject::VERTICALLY_MOVING_PLATFORM_OBJECT_TYPE)
            {
                static_cast<VerticallyMovingPlatformObject *>((*platforms)[i])->updateDuration(duration);
                static_cast<VerticallyMovingPlatformObject *>((*platforms)[i])->run();
            }
            else if ((*platforms)[i]->getType() == GameObject::HORIZONTALLY_MOVING_PLATFORM_OBJECT_TYPE)
            {
                static_cast<HorizontallyMovingPlatformObject *>((*platforms)[i])->updateDuration(duration);
                static_cast<HorizontallyMovingPlatformObject *>((*platforms)[i])->run();
            };
        }

        playersMutex->lock();
        userInputsMutex->lock();
        if (userInputs->size() > 0)
        {
            // Pop the first subscribed user input message from the queue list
            std::string inputMessage = userInputs->front();
            userInputs->erase(userInputs->begin());

            // Parse the received subscription message from one of the player clients
            std::stringstream streamMsg(inputMessage);
            std::string clientIndex;
            std::getline(streamMsg, clientIndex, ',');
            int cIndex = stoi(clientIndex);
            std::string inputs;
            std::getline(streamMsg, inputs);

            if (inputs == "Exit")
            {
                for (int i = 0; i < players->size(); i++)
                {
                    if ((*players)[i]->getIndex() == cIndex)
                    {
                        players->erase(players->begin() + i);
                    }
                }
            }
            else
            {
                // Retrieve the client index from the received client number
                for (int i = 0; i < players->size(); i++)
                {
                    if ((*players)[i]->getIndex() == cIndex)
                    {
                        eventManager->registerEvent(new EventObject(EventObject::CHARACTER_INPUT,
                                                                    EventManager::CHARACTER_INPUT_PRIORITY,
                                                                    (*players)[i]->getHeader(),
                                                                    inputs));
                    }
                }
            }
        }
        // else
        // {
        //     for (int i = 0; i < players->size(); i++)
        //     {
        //         eventManager->registerEvent(new EventObject(EventObject::CHARACTER_INPUT,
        //                                                     EventManager::CHARACTER_INPUT_PRIORITY,
        //                                                     (*players)[i]->getHeader(),
        //                                                     ""));
        //     }
        // }

        // Listen to received and created events
        for (int i = 0; i < players->size(); i++)
        {
            (*players)[i]->checkInputEvent();
            (*players)[i]->checkPlatformCollideEvent();
        }
        userInputsMutex->unlock();

        std::string response = "";
        // Includes current position of moving platform
        for (int i = 0; i < platforms->size(); i++)
        {
            if ((*platforms)[i]->getType() == GameObject::VERTICALLY_MOVING_PLATFORM_OBJECT_TYPE)
            {
                response += static_cast<VerticallyMovingPlatformObject *>((*platforms)[i])->createNetworkMessage();
                response += ",";
            }
            else if ((*platforms)[i]->getType() == GameObject::HORIZONTALLY_MOVING_PLATFORM_OBJECT_TYPE)
            {
                response += static_cast<HorizontallyMovingPlatformObject *>((*platforms)[i])->createNetworkMessage();
                response += ",";
            }
        }
        // Includes player character information including the color and position
        for (int i = 0; i < players->size(); i++)
        {
            response += (*players)[i]->createNetworkMessage();
            response += ",";
        }
        playersMutex->unlock();

        publishStateSocket.send(zmq::buffer(response), zmq::send_flags::none);
        // std::cout << response << std::endl
        //           << std::endl;
    }
}

static void split(std::string str, char deliminer, std::vector<std::string> &splitted);

int main()
{
    // Initializing response socket
    zmq::context_t context{1};
    zmq::socket_t clientConnectionSocket{context, zmq::socket_type::rep};
    clientConnectionSocket.bind("tcp://*:5000");

    // Keeping track of player client subscription threads
    std::vector<std::thread *> clientConnectionThreads;

    // Will push any user inputs with the client number. Declared mutex for thread safety
    std::mutex userInputsMutex;
    std::vector<std::string> userInputs;

    // Character list that represents each player client in the game
    std::vector<CharacterObject *> players;
    std::mutex playersMutex;

    EventManager *eventManager = new EventManager();

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
    VerticallyMovingPlatformObject *verticallyMovingPlatform1 = new VerticallyMovingPlatformObject(eventManager, verticallyMovingPlatformShape1, 1, 400);
    // Vertically moving platform 2
    sf::RectangleShape *verticallyMovingPlatformShape2 = new sf::RectangleShape(sf::Vector2f(MOVING_PLATFORM_WIDTH, PLATFORM_HEIGHT));
    verticallyMovingPlatformShape2->setPosition(1100, WINDOW_HEIGHT / 2);
    VerticallyMovingPlatformObject *verticallyMovingPlatform2 = new VerticallyMovingPlatformObject(eventManager, verticallyMovingPlatformShape2, 2, 400);
    // Horizontally moving platform 1
    sf::RectangleShape *horizontallyMovingPlatformShape1 = new sf::RectangleShape(sf::Vector2f(MOVING_PLATFORM_WIDTH, PLATFORM_HEIGHT));
    horizontallyMovingPlatformShape1->setPosition(850, 100);
    HorizontallyMovingPlatformObject *horizontallyMovingPlatform1 = new HorizontallyMovingPlatformObject(eventManager, horizontallyMovingPlatformShape1, 1, 300);
    // Platforms
    std::vector<GameObject *> platforms;
    platforms.push_back(staticPlatform1);
    platforms.push_back(staticPlatform2);
    platforms.push_back(verticallyMovingPlatform1);
    platforms.push_back(verticallyMovingPlatform2);
    platforms.push_back(horizontallyMovingPlatform1);

    // Spawn/Death Zone
    sf::RectangleShape *spawnZoneShape = new sf::RectangleShape(sf::Vector2f(CHARACTER_SIZE, CHARACTER_SIZE));
    spawnZoneShape->setPosition(100, WINDOW_HEIGHT / 2 - 200);
    spawnZoneShape->setFillColor(sf::Color::Green);
    sf::RectangleShape *deathZoneShape = new sf::RectangleShape(sf::Vector2f(WINDOW_WIDTH * 5, PLATFORM_HEIGHT));
    deathZoneShape->setPosition(-WINDOW_WIDTH, WINDOW_HEIGHT + PLATFORM_HEIGHT);
    deathZoneShape->setFillColor(sf::Color::Red);
    DeathZoneObject *deathZone = new DeathZoneObject(eventManager, deathZoneShape, spawnZoneShape, &players);

    // Publish game state which includes moving platforms and user input response
    std::thread publishStateThread = std::thread(publishGameState, eventManager, &userInputs, &platforms, &players, deathZone, &userInputsMutex, &playersMutex);

    std::cout << "Running Game Server" << std::endl;
    bool isRunning = true;
    int clientIndex = 0;
    while (isRunning)
    {
        // Create a socket that listens to any request and responses to the player client
        zmq::message_t request;
        clientConnectionSocket.recv(request, zmq::recv_flags::none);
        std::string msg = request.to_string();

        // Parse the received request
        std::vector<std::string> tokenized;
        split(msg, ',', tokenized);
        if (tokenized[0] == CLIENT_CONNECTION_REQUEST_HEADER)
        {
            // Received client port number
            int clientPort = stoi(tokenized[1]);

            // Create player object
            // Character
            sf::RectangleShape *characterBoundary = new sf::RectangleShape(sf::Vector2f(WINDOW_WIDTH * 4, WINDOW_HEIGHT));
            characterBoundary->setPosition(-WINDOW_WIDTH, 0);
            sf::RectangleShape *characterShape = new sf::RectangleShape(sf::Vector2f(CHARACTER_SIZE, CHARACTER_SIZE));
            characterShape->setPosition(100, (WINDOW_HEIGHT - CHARACTER_SIZE) / 2);

            playersMutex.lock();
            CharacterObject *player = new CharacterObject(eventManager, characterShape, characterBoundary, clientIndex, CHARACTER_COLORS[clientIndex % CHARACTER_COLORS.size()], &platforms);
            players.push_back(player);
            playersMutex.unlock();

            // Create subscription to the client
            std::thread *clientConnectionThread = new std::thread(subscribeClientInput, clientPort, &userInputs, &userInputsMutex);
            clientConnectionThreads.push_back(clientConnectionThread);

            // Response to the player client's req
            std::string response = SERVER_CONNECTION_RESPONSE_HEADER;
            // Includes the client number of the requested player client
            response += "," + std::to_string(clientIndex);
            // Includes current position of moving platform
            for (int i = 0; i < platforms.size(); i++)
            {

                if (platforms[i]->getType() == GameObject::VERTICALLY_MOVING_PLATFORM_OBJECT_TYPE)
                {
                    response += ",";
                    response += static_cast<VerticallyMovingPlatformObject *>(platforms[i])->createNetworkMessage();
                }
                if (platforms[i]->getType() == GameObject::HORIZONTALLY_MOVING_PLATFORM_OBJECT_TYPE)
                {
                    response += ",";
                    response += static_cast<HorizontallyMovingPlatformObject *>(platforms[i])->createNetworkMessage();
                }
            }
            // Includes player character information including the color and position
            playersMutex.lock();
            for (int i = 0; i < players.size(); i++)
            {
                response += ",";
                response += players[i]->createNetworkMessage();
            }
            playersMutex.unlock();

            clientConnectionSocket.send(zmq::buffer(response), zmq::send_flags::none);
            clientIndex++;
        }
    }

    publishStateThread.join();
    for (int i = 0; i < clientConnectionThreads.size(); i++)
    {
        clientConnectionThreads[i]->join();
    }

    return 0;
}

void split(std::string str, char deliminer, std::vector<std::string> &splitted)
{
    std::stringstream streamMsg(str);
    std::string token;
    while (std::getline(streamMsg, token, deliminer))
    {
        splitted.push_back(token);
    }
}