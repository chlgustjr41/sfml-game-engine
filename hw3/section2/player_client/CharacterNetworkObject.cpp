#include <SFML/Graphics.hpp>
#include "NetworkObject.h"
#include "CharacterNetworkObject.h"

const std::vector<sf::Color> CharacterNetworkObject::CHARACTER_COLORS = {sf::Color::Red, sf::Color::Blue, sf::Color::Green, sf::Color::Yellow, sf::Color::Magenta};

CharacterNetworkObject::CharacterNetworkObject(std::string objectType,
                                               int initialIndex,
                                               sf::RectangleShape *networkObject,
                                               sf::Color initialColor) : NetworkObject(objectType, initialIndex, networkObject)
{
    NetworkObject::getNetworkObject()->setFillColor(initialColor);
}

std::string CharacterNetworkObject::getHeader()
{
    return NetworkObject::getHeader();
}

void CharacterNetworkObject::receiveNetworkMessage(std::string message)
{
    std::string header = getHeader();
    if (message.find(header) != std::string::npos)
    {
        int color;
        float positionX;
        float positionY;
        sscanf(message.substr(header.length()).c_str(), "-Color:%d-Position:%f/%f", &color, &positionX, &positionY);

        NetworkObject::getNetworkObject()->setPosition(positionX, positionY);
        for (int i = 0; i < CHARACTER_COLORS.size(); i++)
        {
            if (CHARACTER_COLORS[i].toInteger() == color)
            {
                NetworkObject::getNetworkObject()->setFillColor(CHARACTER_COLORS[i]);
                break;
            }
        }
    }
}

std::string CharacterNetworkObject::createNetworkMessage()
{
    std::string response = "";
    response += getHeader();
    response += NetworkObject::FIELD_DELIMITER + "Color" + NetworkObject::VALUE_DELIMITER +
                std::to_string(NetworkObject::getNetworkObject()->getFillColor().toInteger());
    response += NetworkObject::FIELD_DELIMITER + "Position" + NetworkObject::VALUE_DELIMITER +
                std::to_string(NetworkObject::getNetworkObject()->getPosition().x) +
                NetworkObject::ANOTHER_VALUE_DELIMITER +
                std::to_string(NetworkObject::getNetworkObject()->getPosition().y);
    return response;
}
