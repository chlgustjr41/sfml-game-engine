#include <SFML/Graphics.hpp>
#include <mutex>

#ifndef THREADOBJECT_H
#define THREADOBJECT_H

class ThreadObject : public sf::RectangleShape
{

public:
    // const std::string CHARACTER_TYPE = "character";
    // const std::string PLATFORM_TYPE = "platform";
    // const std::string MOVING_PLATFORM_TYPE = "moving platform";
    // const std::string UNKNOWN_TYPE = "unknown";
    static const std::string CHARACTER_TYPE;
    static const std::string PLATFORM_TYPE;
    static const std::string MOVING_PLATFORM_TYPE;
    static const std::string UNKNOWN_TYPE;
    static const double TIME_RATIO;

    explicit ThreadObject(const std::string name = UNKNOWN_TYPE,
                          const sf::Vector2f &wSize = sf::Vector2f(0.f, 0.f),
                          const sf::Vector2f &size = sf::Vector2f(0.f, 0.f),
                          const sf::Vector2f &position = sf::Vector2f(0.f, 0.f));

    std::string getThreadName();
    sf::Vector2f getWindowSize();
    double getDuration();
    void setWindowSize(sf::Vector2f size);
    void setDuration(double newDuration);

    virtual void run() = 0;

private:
    sf::Vector2f windowSize;
    std::string threadName;
    sf::Vector2f m_size;
    double duration;
};

#endif