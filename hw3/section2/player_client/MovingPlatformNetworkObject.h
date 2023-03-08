#include <SFML/Graphics.hpp>
#include "NetworkObject.h"

#ifndef MOVINGPLATFORMNETWORKOBJECT_H
#define MOVINGPLATFORMNETWORKOBJECT_H

class MovingPlatformNetworkObject : public NetworkObject
{
public:
    explicit MovingPlatformNetworkObject(std::string objectType,
                                         int initialIndex,
                                         sf::RectangleShape *networkObject);

    std::string getHeader();

    void receiveNetworkMessage(std::string message);
    std::string createNetworkMessage();

private:
};

#endif