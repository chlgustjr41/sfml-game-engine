#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include <chrono>
#include <thread>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <zmq.hpp>
#include "CharacterObject.h"
#include "Timeline.h"
#include "StaticPlatformObject.h"
#include "VerticallyMovingPlatformObject.h"
#include "HorizontallyMovingPlatformObject.h"
#include "DeathZoneObject.h"
#include "ViewObject.h"

std::string const CLIENT_CONNECTION_REQUEST_HEADER = "Client Connection Request";

// Game default constant values
const float WINDOW_WIDTH = 800.0;
const float WINDOW_HEIGHT = 600.0;
const float CHARACTER_SIZE = 75.0;
const float PLATFORM_HEIGHT = 50.0;
const float MOVING_PLATFORM_WIDTH = 100.0;
const std::vector<sf::Color> CHARACTER_COLORS = {sf::Color::Red, sf::Color::Blue, sf::Color::Green, sf::Color::Yellow, sf::Color::Magenta};

void split(std::string str, char deliminer, std::vector<std::string> &splitted);

// For enforcing execution order to draw and then subscribe
std::mutex drawMutex;
std::condition_variable cv;

void subscribeServerUpdate(bool *isRunning,
                           std::vector<CharacterObject *> *players,
                           std::vector<GameObject *> *movingPlatforms,
                           std::mutex *playersMutex,
                           std::mutex *movingPlatformsMutex)
{
    zmq::context_t context{1};

    zmq::socket_t subscribeServerUpdateSocket{context, zmq::socket_type::sub};
    int conflate = 1;
    subscribeServerUpdateSocket.setsockopt(ZMQ_CONFLATE, &conflate, sizeof(conflate));
    subscribeServerUpdateSocket.connect("tcp://localhost:5001");
    subscribeServerUpdateSocket.setsockopt(ZMQ_SUBSCRIBE, "", 0);
    while (*isRunning)
    {
        std::unique_lock<std::mutex> lck(drawMutex);
        zmq::message_t data;
        subscribeServerUpdateSocket.recv(data, zmq::recv_flags::none);

        // Parsing the response
        std::vector<std::string> tokenized;
        split(data.to_string(), ',', tokenized);

        int tokenIndex = 0;
        std::vector<int> existingPlayerIndex;
        while (tokenIndex < tokenized.size())
        {
            std::string message = tokenized[tokenIndex];
            if (message.find("MovingPlatform") != std::string::npos)
            {
                std::string header;
                std::stringstream streamMsg(message);
                std::getline(streamMsg, header, '-');

                movingPlatformsMutex->lock();
                for (int i = 0; i < movingPlatforms->size(); i++)
                {
                    if ((*movingPlatforms)[i]->getType() == GameObject::VERTICALLY_MOVING_PLATFORM_OBJECT_TYPE &&
                        static_cast<VerticallyMovingPlatformObject *>((*movingPlatforms)[i])->getHeader() == header)
                    {
                        static_cast<VerticallyMovingPlatformObject *>((*movingPlatforms)[i])->receiveNetworkMessage(message);
                    }
                    else if ((*movingPlatforms)[i]->getType() == GameObject::HORIZONTALLY_MOVING_PLATFORM_OBJECT_TYPE &&
                             static_cast<HorizontallyMovingPlatformObject *>((*movingPlatforms)[i])->getHeader() == header)
                    {
                        static_cast<HorizontallyMovingPlatformObject *>((*movingPlatforms)[i])->receiveNetworkMessage(message);
                    }
                }
                movingPlatformsMutex->unlock();
            }
            else if (message.find(GameObject::CHARACTER_OBJECT_TYPE) != std::string::npos)
            {
                int playerIndex = -1;
                sscanf(message.c_str(), "character%d", &playerIndex);
                existingPlayerIndex.push_back(playerIndex);

                playersMutex->lock();
                int localPlayerIndex = -1;
                for (int i = 0; i < players->size(); i++)
                {
                    if ((*players)[i]->getIndex() == playerIndex)
                    {
                        localPlayerIndex = i;
                        break;
                    }
                }

                if (localPlayerIndex == -1)
                {
                    sf::RectangleShape *characterBoundary = new sf::RectangleShape(sf::Vector2f(WINDOW_WIDTH * 4, WINDOW_HEIGHT));
                    characterBoundary->setPosition(-WINDOW_WIDTH, 0);
                    sf::RectangleShape *characterShape = new sf::RectangleShape(sf::Vector2f(CHARACTER_SIZE, CHARACTER_SIZE));
                    characterShape->setPosition(100, (WINDOW_HEIGHT - CHARACTER_SIZE) / 2);

                    CharacterObject *addingPlayer = new CharacterObject(characterShape, characterBoundary, playerIndex);
                    addingPlayer->receiveNetworkMessage(message);

                    players->push_back(addingPlayer);
                }
                else
                {
                    (*players)[localPlayerIndex]->receiveNetworkMessage(message);
                }
                playersMutex->unlock();
            }

            tokenIndex++;
        }

        while (players->size() > existingPlayerIndex.size())
        {
            for (int i = 0; i < players->size(); i++)
            {
                bool isExist = false;
                for (int j = 0; j < existingPlayerIndex.size(); j++)
                {
                    if ((*players)[i]->getIndex() == existingPlayerIndex[j])
                    {
                        isExist = true;
                        break;
                    }
                }

                if (!isExist)
                    players->erase(players->begin() + i);
            }
        }

        cv.wait(lck);
    }
}

int main(int argc, char *argv[])
{
    using namespace std::chrono_literals;

    // Number of repeated publish input message for smooth animation
    const float BURST_PUBLISH_COUNT = 1;

    // Initializing request socket
    zmq::context_t context{1};
    zmq::socket_t requestSocket{context, zmq::socket_type::req};
    // Connecting to game server response socket
    requestSocket.connect("tcp://localhost:5000");

    // Initializing publish socket. Dynamically set the port number which changes based on number of player clients running.
    int port = -1;
    zmq::socket_t publisherSocket{context, zmq::socket_type::pub};
    publisherSocket.bind("tcp://0.0.0.0:*");

    char portStr[1024];
    size_t size = sizeof(portStr);
    publisherSocket.getsockopt(ZMQ_LAST_ENDPOINT, &portStr, &size);
    sscanf(portStr, "tcp://0.0.0.0:%d", &port);

    // Setting up local game state
    sf::RenderWindow *window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Player Port: " + std::to_string(port));
    window->setFramerateLimit(60);

    // Static platform 1
    sf::RectangleShape *staticPlatformShape1 = new sf::RectangleShape(sf::Vector2f(WINDOW_WIDTH, PLATFORM_HEIGHT));
    staticPlatformShape1->setPosition(0, WINDOW_HEIGHT - PLATFORM_HEIGHT);
    StaticPlatformObject *staticPlatform1 = new StaticPlatformObject(staticPlatformShape1);
    // Static platform 2
    sf::RectangleShape *staticPlatformShape2 = new sf::RectangleShape(sf::Vector2f(WINDOW_WIDTH, PLATFORM_HEIGHT));
    staticPlatformShape2->setPosition(WINDOW_WIDTH + CHARACTER_SIZE + 100, WINDOW_HEIGHT - PLATFORM_HEIGHT);
    StaticPlatformObject *staticPlatform2 = new StaticPlatformObject(staticPlatformShape2);
    // Static Platforms
    std::vector<StaticPlatformObject *> staticPlatforms;
    staticPlatforms.push_back(staticPlatform1);
    staticPlatforms.push_back(staticPlatform2);

    // Vertically moving platform 1
    sf::RectangleShape *verticallyMovingPlatformShape1 = new sf::RectangleShape(sf::Vector2f(MOVING_PLATFORM_WIDTH, PLATFORM_HEIGHT));
    verticallyMovingPlatformShape1->setPosition(300, WINDOW_HEIGHT / 2);
    VerticallyMovingPlatformObject *verticallyMovingPlatform1 = new VerticallyMovingPlatformObject(verticallyMovingPlatformShape1, 1, 400);
    // Vertically moving platform 2
    sf::RectangleShape *verticallyMovingPlatformShape2 = new sf::RectangleShape(sf::Vector2f(MOVING_PLATFORM_WIDTH, PLATFORM_HEIGHT));
    verticallyMovingPlatformShape2->setPosition(1100, WINDOW_HEIGHT / 2);
    VerticallyMovingPlatformObject *verticallyMovingPlatform2 = new VerticallyMovingPlatformObject(verticallyMovingPlatformShape2, 2, 400);
    // Horizontally moving platform 1
    sf::RectangleShape *horizontallyMovingPlatformShape1 = new sf::RectangleShape(sf::Vector2f(MOVING_PLATFORM_WIDTH, PLATFORM_HEIGHT));
    horizontallyMovingPlatformShape1->setPosition(850, 100);
    HorizontallyMovingPlatformObject *horizontallyMovingPlatform1 = new HorizontallyMovingPlatformObject(horizontallyMovingPlatformShape1, 1, 300);
    // Moving Platforms
    std::vector<GameObject *> movingPlatforms;
    movingPlatforms.push_back(verticallyMovingPlatform1);
    movingPlatforms.push_back(verticallyMovingPlatform2);
    movingPlatforms.push_back(horizontallyMovingPlatform1);
    // Mutex for Moving Platforms
    std::mutex movingPlatformsMutex;

    // List of players in the game
    std::vector<CharacterObject *> players;
    std::mutex playersMutex;

    std::cout << "Sending message" << std::endl;
    // Sending a request to the game server response socket
    std::string request = CLIENT_CONNECTION_REQUEST_HEADER + "," + std::to_string(port);
    requestSocket.send(zmq::buffer(request), zmq::send_flags::none);

    // Receiving a response from the game server response socket
    zmq::message_t response;
    requestSocket.recv(response, zmq::recv_flags::none);
    std::cout << "Response =>" << response.to_string() << std::endl;

    // Parsing the response
    std::vector<std::string> tokenized;
    split(response.to_string(), ',', tokenized);

    // Retrieving this player client's client number
    std::string clientNumber = tokenized[1];
    int localCurrentClientIndex = -1;

    // Updating each GameObject state
    int tokenIndex = 2;
    while (tokenIndex < tokenized.size())
    {
        std::string message = tokenized[tokenIndex];
        if (message.find(GameObject::CHARACTER_OBJECT_TYPE) != std::string::npos)
        {
            int playerIndex = -1;
            sscanf(message.c_str(), "character%d", &playerIndex);

            sf::RectangleShape *characterBoundary = new sf::RectangleShape(sf::Vector2f(WINDOW_WIDTH * 4, WINDOW_HEIGHT));
            characterBoundary->setPosition(-WINDOW_WIDTH, 0);
            sf::RectangleShape *characterShape = new sf::RectangleShape(sf::Vector2f(CHARACTER_SIZE, CHARACTER_SIZE));
            characterShape->setPosition(100, (WINDOW_HEIGHT - CHARACTER_SIZE) / 2);

            CharacterObject *addingPlayer = new CharacterObject(characterShape, characterBoundary, playerIndex);
            addingPlayer->receiveInput(tokenized[tokenIndex]);

            if (std::to_string(playerIndex) == clientNumber)
            {
                localCurrentClientIndex = players.size();
            }

            playersMutex.lock();
            players.push_back(addingPlayer);
            playersMutex.unlock();
        }
        else if (message.find("MovingPlatform") != std::string::npos)
        {
            std::string header;
            std::stringstream streamMsg(message);
            std::getline(streamMsg, header, '-');

            movingPlatformsMutex.lock();
            for (int i = 0; i < movingPlatforms.size(); i++)
            {
                if (movingPlatforms[i]->getType() == GameObject::VERTICALLY_MOVING_PLATFORM_OBJECT_TYPE &&
                    static_cast<VerticallyMovingPlatformObject *>(movingPlatforms[i])->getHeader() == header)
                {
                    static_cast<VerticallyMovingPlatformObject *>(movingPlatforms[i])->receiveNetworkMessage(message);
                }
                else if (movingPlatforms[i]->getType() == GameObject::HORIZONTALLY_MOVING_PLATFORM_OBJECT_TYPE &&
                         static_cast<HorizontallyMovingPlatformObject *>(movingPlatforms[i])->getHeader() == header)
                {
                    static_cast<HorizontallyMovingPlatformObject *>(movingPlatforms[i])->receiveNetworkMessage(message);
                }
            }
            movingPlatformsMutex.unlock();
        }

        tokenIndex++;
    }

    // View adjuster for the character
    sf::RectangleShape *localClient = players[localCurrentClientIndex]->getMovableObject()->getMovingObject();
    sf::RectangleShape *viewBoundary = new sf::RectangleShape(sf::Vector2f(CHARACTER_SIZE * 2, CHARACTER_SIZE * 2));
    viewBoundary->setPosition(localClient->getPosition().x - (viewBoundary->getSize().x - localClient->getSize().x) / 2,
                              localClient->getPosition().y - (viewBoundary->getSize().y - localClient->getSize().y) / 2);
    viewBoundary->setFillColor(sf::Color::Blue);
    ViewObject *view = new ViewObject(viewBoundary, localClient);
    // Set default starting view
    sf::View windowView = window->getView();
    windowView.setCenter(localClient->getPosition().x, windowView.getCenter().y);
    window->setView(windowView);

    // Set up a thread to subscribe to the game state update
    bool isRunning = true;
    std::thread subscribeServerUpdateThread = std::thread(subscribeServerUpdate, &isRunning, &players, &movingPlatforms, &playersMutex, &movingPlatformsMutex);
    while (isRunning)
    {
        if (window->hasFocus())
        {
            window->setActive(true);
            std::unique_lock<std::mutex> lck(drawMutex);

            view->updateView(window);

            sf::Event event;
            while (window->pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    window->setActive(false);
                    window->close();
                    isRunning = false;
                    publisherSocket.send(zmq::buffer(clientNumber + ",Exit"), zmq::send_flags::none);
                }
            }

            std::string inputMsg = clientNumber;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                inputMsg += ",Space";
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                inputMsg += ",Left";
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                inputMsg += ",Right";
            }
            if (inputMsg.length() > 2)
            {
                for (int i = 0; i < BURST_PUBLISH_COUNT; i++)
                    publisherSocket.send(zmq::buffer(inputMsg), zmq::send_flags::none);
                std::cout << inputMsg << std::endl;
            }

            window->clear(sf::Color::Black);

            // Rendering all static platforms
            for (int i = 0; i < staticPlatforms.size(); i++)
                staticPlatforms[i]->render(window);

            // Rendering all moving platforms
            movingPlatformsMutex.lock();
            for (int i = 0; i < movingPlatforms.size(); i++)
                if (movingPlatforms[i]->getType() == GameObject::VERTICALLY_MOVING_PLATFORM_OBJECT_TYPE)
                {
                    static_cast<VerticallyMovingPlatformObject *>(movingPlatforms[i])->render(window);
                }
                else if (movingPlatforms[i]->getType() == GameObject::HORIZONTALLY_MOVING_PLATFORM_OBJECT_TYPE)
                {
                    static_cast<HorizontallyMovingPlatformObject *>(movingPlatforms[i])->render(window);
                }
            movingPlatformsMutex.unlock();

            // Rendering all players
            playersMutex.lock();
            for (int i = 0; i < players.size(); i++)
                players[i]->render(window);
            playersMutex.unlock();

            window->display();
            cv.notify_all();
        }
        window->setActive(false);
    }

    subscribeServerUpdateThread.detach();

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