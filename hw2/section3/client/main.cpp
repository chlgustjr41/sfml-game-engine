#include <SFML/Graphics.hpp>
#include <string>
#include <chrono>
#include <thread>
#include <iostream>
#include <sstream>
#include <zmq.hpp>

std::string const CLIENT_CONNECTION_REQUEST_HEADER = "Client Connection Request";
std::string const REQUEST_DELIMITER = ",";

void subscribeServerUpdate()
{
    zmq::context_t context{1};

    zmq::socket_t subscribeServerUpdateSocket{context, zmq::socket_type::sub};
    subscribeServerUpdateSocket.connect("tcp://localhost:5001");
    subscribeServerUpdateSocket.setsockopt(ZMQ_SUBSCRIBE, "", 0);
    std::cout << "Server Subscribe Thread: Connected to server publisher." << std::endl;

    bool isRunning = true;
    while (isRunning)
    {
        zmq::message_t data;
        subscribeServerUpdateSocket.recv(data, zmq::recv_flags::none);
        std::cout << data.to_string() << std::endl;
    }
}

int main(int argc, char *argv[])
{
    using namespace std::chrono_literals;

    zmq::context_t context{1};

    std::cout << "Main Thread: Connecting to the server with port 5000..." << std::endl;
    zmq::socket_t requestSocket{context, zmq::socket_type::req};
    requestSocket.connect("tcp://localhost:5000");

    std::cout << "Main Thread: Establishing publisher..." << std::endl;
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
    std::cout << "Main Thread: Publisher with port " << port << " has established!" << std::endl;

    std::cout << "Main Thread: Requesting to join the server..." << std::endl;
    std::string request = CLIENT_CONNECTION_REQUEST_HEADER + REQUEST_DELIMITER + std::to_string(port);
    requestSocket.send(zmq::buffer(request), zmq::send_flags::none);

    zmq::message_t response;
    requestSocket.recv(response, zmq::recv_flags::none);

    std::stringstream streamMsg(response.to_string());
    std::vector<std::string> tokenized;
    std::string token;
    while (std::getline(streamMsg, token, ','))
    {
        tokenized.push_back(token);
    }
    std::string clientNumber = tokenized[1];
    std::cout << "Main Thread: Connection successful. I am Client " << clientNumber << "." << std::endl;

    std::thread subscribeServerUpdateThread = std::thread(subscribeServerUpdate);

    int messageCount = 0;
    bool isPublishing = true;
    std::cout << "Main Thread: Press ENTER to publish a message to the server." << std::endl;

    while (isPublishing)
    {
        messageCount++;
        std::string userMsg;
        std::getline(std::cin, userMsg);
        publisherSocket.send(zmq::buffer(clientNumber + ",Hello World " + std::to_string(messageCount)), zmq::send_flags::none);
    }

    subscribeServerUpdateThread.join();

    return 0;
}