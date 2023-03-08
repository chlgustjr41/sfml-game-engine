#include <SFML/Graphics.hpp>
#include <string>
#include <chrono>
#include <thread>
#include <iostream>
#include <mutex>
#include <zmq.hpp>

std::string const CLIENT_CONNECTION_REQUEST_HEADER = "Client Connection Request";

// Game default constant values
const float WINDOW_WIDTH = 800.0;
const float WINDOW_HEIGHT = 600.0;
const float CHARACTER_SIZE = 75.0;
const float PLATFORM_HEIGHT = 50.0;
const float MOVING_PLATFORM_WIDTH = 100.0;
const std::vector<sf::Color> CHARACTER_COLORS = {sf::Color::Red, sf::Color::Blue, sf::Color::Green, sf::Color::Yellow, sf::Color::Magenta};

void split(std::string str, char deliminer, std::vector<std::string> &splitted);

void subscribeServerUpdate(std::vector<sf::RectangleShape *> *players, sf::RectangleShape *movingPlatform, std::mutex *playersMutex, std::mutex *movingPlatformMutex)
{
    zmq::context_t context{1};

    zmq::socket_t subscribeServerUpdateSocket{context, zmq::socket_type::sub};
    subscribeServerUpdateSocket.connect("tcp://localhost:5001");
    subscribeServerUpdateSocket.setsockopt(ZMQ_SUBSCRIBE, "", 0);

    bool isRunning = true;
    while (isRunning)
    {
        zmq::message_t data;
        subscribeServerUpdateSocket.recv(data, zmq::recv_flags::none);

        // Parsing the response
        std::vector<std::string> tokenized;
        split(data.to_string(), ',', tokenized);

        int tokenIndex = 0;
        while (tokenIndex < tokenized.size())
        {
            if (tokenized[tokenIndex].find("MovingPlatform") != std::string::npos)
            {
                float positionX;
                float positionY;
                sscanf(tokenized[tokenIndex].c_str(), "MovingPlatform-Position:%f/%f", &positionX, &positionY);
                movingPlatform->setPosition(positionX, positionY);
            }
            else if (tokenized[tokenIndex].find("Player") != std::string::npos)
            {
                int playerIndex;
                int color;
                float positionX;
                float positionY;
                sscanf(tokenized[tokenIndex].c_str(), "Player%d-Color:%d-Position:%f/%f", &playerIndex, &color, &positionX, &positionY);

                playersMutex->lock();
                if (playerIndex >= players->size())
                {
                    sf::RectangleShape *addingPlayer = new sf::RectangleShape(sf::Vector2f(CHARACTER_SIZE, CHARACTER_SIZE));
                    for (int i = 0; i < CHARACTER_COLORS.size(); i++)
                    {
                        if (CHARACTER_COLORS[i].toInteger() == color)
                        {
                            addingPlayer->setFillColor(CHARACTER_COLORS[i]);
                            break;
                        }
                    }
                    addingPlayer->setPosition(positionX, positionY);
                    players->push_back(addingPlayer);
                }
                else
                {
                    (*players)[playerIndex]->setPosition(positionX, positionY);
                }
                playersMutex->unlock();
            }

            tokenIndex++;
        }
    }
}

int main(int argc, char *argv[])
{
    using namespace std::chrono_literals;

    // Number of repeated publish input message for smooth animation
    const float BURST_PUBLISH_COUNT = 25;

    // Initializing request socket
    zmq::context_t context{1};
    zmq::socket_t requestSocket{context, zmq::socket_type::req};
    // Connecting to game server response socket
    requestSocket.connect("tcp://localhost:5000");

    // Initializing publish socket. Dynamically set the port number which changes based on number of player clients running.
    int port = 5010;
    bool isPortUsed = false;
    zmq::socket_t publisherSocket{context, zmq::socket_type::pub};
    while (!isPortUsed)
    {
        try
        {
            publisherSocket.bind("tcp://*:" + std::to_string(port));
            isPortUsed = true;
        }
        catch (const zmq::error_t e)
        {
            port += 10;
        }
    }

    // Setting up local game state
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Player Port: " + std::to_string(port));
    window.setFramerateLimit(60);
    // Setting up local platform state
    sf::RectangleShape platform(sf::Vector2f(WINDOW_WIDTH, PLATFORM_HEIGHT));
    platform.setPosition(0, WINDOW_HEIGHT - PLATFORM_HEIGHT);
    sf::RectangleShape movingPlatform(sf::Vector2f(MOVING_PLATFORM_WIDTH, PLATFORM_HEIGHT));
    movingPlatform.setPosition((WINDOW_WIDTH - MOVING_PLATFORM_WIDTH) / 2, WINDOW_HEIGHT / 2);
    std::mutex movingPlatformMutex;

    // List of players in the game
    std::vector<sf::RectangleShape *> players;
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

    // Retrievign this player client's client number
    std::string clientNumber = tokenized[1];
    int tokenIndex = 2;
    while (tokenIndex < tokenized.size())
    {
        if (tokenized[tokenIndex].find("MovingPlatform") != std::string::npos)
        {
            float positionX;
            float positionY;
            sscanf(tokenized[tokenIndex].c_str(), "MovingPlatform-Position:%f/%f", &positionX, &positionY);
            movingPlatformMutex.lock();
            movingPlatform.setPosition(positionX, positionY);
            movingPlatformMutex.unlock();
        }
        else if (tokenized[tokenIndex].find("Player") != std::string::npos)
        {
            int playerNumber;
            int color;
            float positionX;
            float positionY;
            sscanf(tokenized[tokenIndex].c_str(), "Player%d-Color:%d-Position:%f/%f", &playerNumber, &color, &positionX, &positionY);
            sf::RectangleShape *addingPlayer = new sf::RectangleShape(sf::Vector2f(CHARACTER_SIZE, CHARACTER_SIZE));
            for (int i = 0; i < CHARACTER_COLORS.size(); i++)
            {
                if (CHARACTER_COLORS[i].toInteger() == color)
                {
                    addingPlayer->setFillColor(CHARACTER_COLORS[i]);
                    break;
                }
            }
            addingPlayer->setPosition(positionX, positionY);
            playersMutex.lock();
            players.push_back(addingPlayer);
            playersMutex.unlock();
        }

        tokenIndex++;
    }

    // Set up a thread to subscribe to the game state update
    std::thread subscribeServerUpdateThread = std::thread(subscribeServerUpdate, &players, &movingPlatform, &playersMutex, &movingPlatformMutex);
    bool isRunning = true;
    while (isRunning)
    {
        if (window.hasFocus())
        {
            window.setActive(true);
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    window.close();
                    isRunning = false;
                }

                std::string inputMsg = clientNumber;
                if (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                {
                    inputMsg += ",Left";
                }

                if (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                {
                    inputMsg += ",Right";
                }

                if (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                {
                    inputMsg += ",Space";
                }

                if (inputMsg.length() > 2)
                    for (int i = 0; i < BURST_PUBLISH_COUNT; i++)
                        publisherSocket.send(zmq::buffer(inputMsg), zmq::send_flags::none);
                std::cout << inputMsg << std::endl;
            }

            window.clear(sf::Color::Black);

            window.draw(platform);

            movingPlatformMutex.lock();
            window.draw(movingPlatform);
            movingPlatformMutex.unlock();

            playersMutex.lock();
            for (int i = 0; i < players.size(); i++)
                window.draw(*players[i]);
            playersMutex.unlock();

            window.display();
        }
        window.setActive(false);
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