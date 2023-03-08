#include <SFML/Graphics.hpp>
#include "NetworkObject.h"
#include "MovingPlatformNetworkObject.h"

MovingPlatformNetworkObject::MovingPlatformNetworkObject(std::string objectType,
                                                         int initialIndex,
                                                         sf::RectangleShape *networkObject) : NetworkObject(objectType, initialIndex, networkObject)
{
}

std::string MovingPlatformNetworkObject::getHeader()
{
    return NetworkObject::getHeader();
}

void MovingPlatformNetworkObject::receiveNetworkMessage(std::string message)
{
    std::string header = getHeader();
    if (message.find(header) != std::string::npos)
    {
        float positionX;
        float positionY;
        sscanf(message.substr(header.length()).c_str(), "-Position:%f/%f", &positionX, &positionY);

        NetworkObject::getNetworkObject()->setPosition(positionX, positionY);
    }
}

std::string MovingPlatformNetworkObject::createNetworkMessage()
{
    std::string response = "";
    response += getHeader();
    response += NetworkObject::FIELD_DELIMITER + "Position" + NetworkObject::VALUE_DELIMITER +
                std::to_string(NetworkObject::getNetworkObject()->getPosition().x) +
                NetworkObject::ANOTHER_VALUE_DELIMITER +
                std::to_string(NetworkObject::getNetworkObject()->getPosition().y);
    return response;
}
