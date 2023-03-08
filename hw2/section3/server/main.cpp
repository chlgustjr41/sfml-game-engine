#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include <vector>
#include <chrono>
#include <thread>
#include <mutex>
#include <iostream>
#include <zmq.hpp>

std::string const CLIENT_CONNECTION_REQUEST_HEADER = "Client Connection Request";
std::string const SERVER_CONNECTION_RESPONSE_HEADER = "Server Connection Response";
std::string const REQUEST_DELIMITER = ",";

void subscribeToClient(int port, std::vector<std::string> *updates, std::mutex *mutex)
{
    zmq::context_t context{1};
    zmq::socket_t clientConnection{context, zmq::socket_type::sub};

    clientConnection.connect("tcp://localhost:" + std::to_string(port));
    clientConnection.setsockopt(ZMQ_SUBSCRIBE, "", 0);

    bool isRunning = true;
    while (isRunning)
    {
        zmq::message_t data;
        clientConnection.recv(data, zmq::recv_flags::none);

        mutex->lock();
        updates->push_back(data.to_string());
        mutex->unlock();
    }
}

void publishState(std::vector<std::string> *updates, std::vector<int> *clientIterationCounter, std::mutex *mutex, std::mutex *counterMutex)
{
    zmq::context_t context{1};

    zmq::socket_t publishStateSocket{context, zmq::socket_type::pub};
    publishStateSocket.bind("tcp://*:5001");

    bool isRunning = true;
    while (isRunning)
    {

        mutex->lock();
        if (updates->size() > 0)
        {
            std::string updateMsg = updates->front();
            updates->erase(updates->begin());

            std::stringstream streamMsg(updateMsg);
            std::vector<std::string> tokenized;
            std::string token;
            while (std::getline(streamMsg, token, ','))
            {
                tokenized.push_back(token);
            }
            counterMutex->lock();
            int clientNumber = stoi(tokenized[0]) - 1;
            (*clientIterationCounter)[clientNumber]++;
            counterMutex->unlock();

            std::string publishingMsg = "Client " + tokenized[0] + ": Iteration " + std::to_string((*clientIterationCounter)[clientNumber]) + " - " + tokenized[1];
            publishStateSocket.send(zmq::buffer(publishingMsg), zmq::send_flags::none);
            std::cout << publishingMsg << std::endl;
        }
        mutex->unlock();
    }
}

int main()
{
    using namespace std::chrono_literals;

    std::cout << "Main Thread: The server is running." << std::endl;

    zmq::context_t context{1};

    zmq::socket_t clientConnectionSocket{context, zmq::socket_type::rep};
    clientConnectionSocket.bind("tcp://*:5000");

    std::vector<std::string> clients;
    std::vector<std::thread *> clientConnectionThreads;

    std::mutex counterMutex;
    std::vector<int> clientIterationCounter;

    std::mutex mutex;
    std::vector<std::string> updates;
    std::thread publishStateThread = std::thread(publishState, &updates, &clientIterationCounter, &mutex, &counterMutex);

    bool isRunning = true;
    while (isRunning)
    {
        zmq::message_t request;
        clientConnectionSocket.recv(request, zmq::recv_flags::none);
        std::string msg = request.to_string();

        std::stringstream streamMsg(msg);
        std::vector<std::string> tokenized;
        std::string token;
        while (std::getline(streamMsg, token, ','))
        {
            tokenized.push_back(token);
        }

        if (tokenized[0] == CLIENT_CONNECTION_REQUEST_HEADER)
        {
            clients.push_back("Client " + (clients.size() + 1));

            int clientPort = stoi(tokenized[1]);

            std::string response = SERVER_CONNECTION_RESPONSE_HEADER + REQUEST_DELIMITER + std::to_string(clients.size());
            clientConnectionSocket.send(zmq::buffer(response), zmq::send_flags::none);

            std::thread *clientConnectionThread = new std::thread(subscribeToClient, clientPort, &updates, &mutex);
            clientConnectionThreads.push_back(clientConnectionThread);

            counterMutex.lock();
            clientIterationCounter.push_back(0);
            counterMutex.unlock();

            std::cout << "Main Thread: Client " << clients.size() << " has joined!" << std::endl;
        }
    }

    publishStateThread.join();
    for (int i = 0; i < clientConnectionThreads.size(); i++)
    {
        clientConnectionThreads[i]->join();
    }

    return 0;
}