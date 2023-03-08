#include <SFML/Graphics.hpp>
#include "NetworkObject.h"

const std::string NetworkObject::OBJECT_DELIMITER = ",";
const std::string NetworkObject::FIELD_DELIMITER = "-";
const std::string NetworkObject::VALUE_DELIMITER = ":";
const std::string NetworkObject::ANOTHER_VALUE_DELIMITER = "/";

NetworkObject::NetworkObject(std::string objectType,
                             int initialIndex,
                             sf::RectangleShape *initialNetworkObject)
{
    type = objectType;
    index = initialIndex;
    networkObject = initialNetworkObject;
}

std::string NetworkObject::getType()
{
    return type;
}
sf::RectangleShape *NetworkObject::getNetworkObject()
{
    return networkObject;
}
int NetworkObject::getIndex()
{
    return index;
}

void NetworkObject::setType(std::string newType)
{
    type = newType;
}
void NetworkObject::setNetworkObject(sf::RectangleShape *newNetworkObject)
{
    networkObject = newNetworkObject;
}
void NetworkObject::setIndex(int newIndex)
{
    index = newIndex;
}

std::string NetworkObject::getHeader()
{
    return getType() + std::to_string(getIndex());
}