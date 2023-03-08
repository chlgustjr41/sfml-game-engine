#include <SFML/Graphics.hpp>
#include "NetworkObject.h"

#ifndef CHARACTERNETWORKOBJECT_H
#define CHARACTERNETWORKOBJECT_H

class CharacterNetworkObject : public NetworkObject
{
public:
    static const std::vector<sf::Color> CHARACTER_COLORS;

    explicit CharacterNetworkObject(std::string objectType,
                                    int initialIndex,
                                    sf::RectangleShape *networkObject,
                                    sf::Color initialColor);

    std::string getHeader();

    void receiveNetworkMessage(std::string message);
    std::string createNetworkMessage();

private:
};

#endif