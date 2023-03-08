#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include <vector>
#include <chrono>
#include <thread>
#include <mutex>
#include <iostream>
#include <zmq.hpp>

#include "PlatformThread.h"
#include "MovingPlatformThread.h"
#include "CharacterThread.h"
#include "Timeline.h"

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

void handleThreadObjectRun(ThreadObject *obj)
{
    obj->run();
}

void subscribeClientInput(int port, std::vector<std::string> *userInputs, std::mutex *userInputsMutex)
{
    // Initializing publish subscribe
    zmq::context_t context{1};
    zmq::socket_t clientConnection{context, zmq::socket_type::sub};
    // Connecting and subscribing to player client publish socket
    clientConnection.connect("tcp://localhost:" + std::to_string(port));
    clientConnection.setsockopt(ZMQ_SUBSCRIBE, "", 0);

    bool isRunning = true;
    while (isRunning)
    {
        zmq::message_t data;
        clientConnection.recv(data, zmq::recv_flags::none);
        // std::cout << data.to_string() << std::endl;

        userInputsMutex->lock();
        userInputs->push_back(data.to_string());
        userInputsMutex->unlock();
    }
}

void publishGameState(std::vector<std::string> *userInputs, ThreadObject **platforms, std::vector<CharacterThread *> *players,
                      std::mutex *userInputsMutex, std::mutex *playersMutex)
{
    using namespace std::chrono_literals;
    const double DEFAULT_TICK = 1;

    // Initializing publish socket
    zmq::context_t context{1};
    zmq::socket_t publishStateSocket{context, zmq::socket_type::pub};
    publishStateSocket.bind("tcp://*:5001");

    Timeline timeline(DEFAULT_TICK);

    bool isRunning = true;
    while (isRunning)
    {
        std::this_thread::sleep_for(1ms); // Optimized thread execution time
        double duration = timeline.getDuration();

        playersMutex->lock();
        userInputsMutex->lock();
        if (userInputs->size() > 0)
        {
            // Pop the first subscribed user input message from the queue list
            std::string inputMessage = userInputs->front();
            userInputs->erase(userInputs->begin());

            // Parse the received subscription message from one of the player clients
            std::stringstream streamMsg(inputMessage);
            std::vector<std::string> tokenized;
            std::string token;
            while (std::getline(streamMsg, token, ','))
            {
                tokenized.push_back(token);
            }

            // Retrieve the client index from the received client number
            int clientIndex = stoi(tokenized[0]) - 1;
            int tokenIndex = 1;
            while (tokenIndex < tokenized.size())
            {
                // Check the type of input and update player character state in the game
                if (tokenized[tokenIndex] == "Left")
                {
                    (*players)[clientIndex]->setIsMovingLeft(true);
                }
                else
                {
                    (*players)[clientIndex]->setIsMovingLeft(false);
                }
                if (tokenized[tokenIndex] == "Right")
                {
                    (*players)[clientIndex]->setIsMovingRight(true);
                }
                else
                {
                    (*players)[clientIndex]->setIsMovingRight(false);
                }
                if (tokenized[tokenIndex] == "Space" && !(*players)[clientIndex]->getIsJumping())
                {
                    (*players)[clientIndex]->setIsJumping(true);
                    (*players)[clientIndex]->move(0, 0.5); // 0.5 jump to get off of intersect state
                }
                tokenIndex++;
            }
        }
        else
        {
            for (int i = 0; i < players->size(); i++)
            {
                (*players)[i]->setIsMovingLeft(false);
                (*players)[i]->setIsMovingRight(false);
            }
        }
        userInputsMutex->unlock();

        // Rest of game object movements needs to be updated
        platforms[0]->setDuration(duration);
        platforms[1]->setDuration(duration);
        std::thread movingPlatformThread(handleThreadObjectRun, platforms[1]);

        for (int i = 0; i < players->size(); i++)
        {
            (*players)[i]->setDuration(duration);
            std::thread playerThread(handleThreadObjectRun, (*players)[i]);
            playerThread.join();
        }
        movingPlatformThread.join();

        // Includes current position of moving platform
        std::string response = "MovingPlatform";
        response += "-Position:" + std::to_string(platforms[1]->getPosition().x) + "/" + std::to_string(platforms[1]->getPosition().y);
        // Includes player character information including the color and position
        for (int i = 0; i < players->size(); i++)
        {
            response += ",Player" + std::to_string(i);
            response += "-Color:" + std::to_string((*players)[i]->getFillColor().toInteger());
            response += "-Position:" + std::to_string((*players)[i]->getPosition().x) + "/" + std::to_string((*players)[i]->getPosition().y);
        }
        playersMutex->unlock();

        publishStateSocket.send(zmq::buffer(response), zmq::send_flags::none);
        std::cout << response << std::endl;
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
    std::vector<CharacterThread *> players;
    std::mutex playersMutex;

    // Initializing game platforms
    PlatformThread platform(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT),
                            sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT),
                            sf::Vector2f(0, WINDOW_HEIGHT - PLATFORM_HEIGHT));
    MovingPlatformThread movingPlatform(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT),
                                        sf::Vector2f(MOVING_PLATFORM_WIDTH, PLATFORM_HEIGHT),
                                        sf::Vector2f((WINDOW_WIDTH - MOVING_PLATFORM_WIDTH) / 2, WINDOW_HEIGHT / 2));

    // Platform list that represents each platform in the game
    ThreadObject *platforms[PLATFORM_COUNT];
    platforms[0] = &platform;
    platforms[1] = &movingPlatform;

    // Publish game state which includes moving platforms and user input response
    std::thread publishStateThread = std::thread(publishGameState, &userInputs, platforms, &players, &userInputsMutex, &playersMutex);

    std::cout << "Running" << std::endl;
    bool isRunning = true;
    while (isRunning)
    {
        // Create a socket that listens to any request and responses to the player client
        zmq::message_t request;
        clientConnectionSocket.recv(request, zmq::recv_flags::none);
        std::string msg = request.to_string();
        std::cout << "Received request => " << msg << std::endl;

        // Parse the received request
        std::vector<std::string> tokenized;
        split(msg, ',', tokenized);
        std::cout << "Tokenized Length => " << tokenized.size() << std::endl;
        if (tokenized[0] == CLIENT_CONNECTION_REQUEST_HEADER)
        {
            // Received client port number
            int clientPort = stoi(tokenized[1]);

            // Create player object
            CharacterThread *player = new CharacterThread(platforms,
                                                          PLATFORM_COUNT,
                                                          sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT),
                                                          sf::Vector2f(CHARACTER_SIZE, CHARACTER_SIZE),
                                                          sf::Vector2f((WINDOW_WIDTH - CHARACTER_SIZE) / 2, WINDOW_HEIGHT - PLATFORM_HEIGHT - CHARACTER_SIZE + 0.5));
            playersMutex.lock();
            player->setFillColor(CHARACTER_COLORS[players.size() % CHARACTER_COLORS.size()]);
            players.push_back(player);
            playersMutex.unlock();

            // Create subscription to the client
            std::thread *clientConnectionThread = new std::thread(subscribeClientInput, clientPort, &userInputs, &userInputsMutex);
            clientConnectionThreads.push_back(clientConnectionThread);

            // Response to the player client's req
            std::string response = SERVER_CONNECTION_RESPONSE_HEADER;
            // Includes the client number of the requested player client
            response += "," + std::to_string(clientConnectionThreads.size());
            // Includes current position of moving platform
            response += ",MovingPlatform";
            response += "-Position:" + std::to_string(platforms[1]->getPosition().x) + "/" + std::to_string(platforms[1]->getPosition().y);
            // Includes player character information including the color and position
            playersMutex.lock();
            for (int i = 0; i < players.size(); i++)
            {
                response += ",Player" + std::to_string(i);
                response += "-Color:" + std::to_string(players[i]->getFillColor().toInteger());
                response += "-Position:" + std::to_string(players[i]->getPosition().x) + "/" + std::to_string(players[i]->getPosition().y);
            }
            playersMutex.unlock();

            clientConnectionSocket.send(zmq::buffer(response), zmq::send_flags::none);
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