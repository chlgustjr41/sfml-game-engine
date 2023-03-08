#ifndef NETWORKOBJECT_H
#define NETWORKOBJECT_H

#include <SFML/Graphics.hpp>

class NetworkObject
{

public:
    static const std::string OBJECT_DELIMITER;
    static const std::string FIELD_DELIMITER;
    static const std::string VALUE_DELIMITER;
    static const std::string ANOTHER_VALUE_DELIMITER;

    explicit NetworkObject(std::string objectType,
                           int initialIndex,
                           sf::RectangleShape *initialNetworkObject);

    virtual void receiveNetworkMessage(std::string message) = 0;
    virtual std::string createNetworkMessage() = 0;

    std::string getType();
    sf::RectangleShape *getNetworkObject();
    int getIndex();

    void setType(std::string newType);
    void setNetworkObject(sf::RectangleShape *newNetworkObject);
    void setIndex(int newIndex);

    std::string getHeader();

private:
    std::string type;
    sf::RectangleShape *networkObject;
    int index;
};

#endif