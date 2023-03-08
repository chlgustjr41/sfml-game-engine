#include <SFML/Graphics.hpp>
#include "PlatformShape.h"

PlatformShape::PlatformShape(const sf::Vector2f& size, 
                        const sf::Vector2f& position) :
m_size(size)
{
    setFillColor(sf::Color::White);
    setPosition(position.x, position.y);
    update();
}

std::size_t PlatformShape::getPointCount() const
{
    return 4;
}

sf::Vector2f PlatformShape::getPoint(std::size_t index) const
{
    int x = 0;
    int y = 0;

    if (index == 0) 
    {
        x = 0;
        y = 0;
    }
    else if (index == 1) 
    {
        x = m_size.x;
        y = 0;
    }
    else if (index == 2) 
    {
        x = m_size.x;
        y = m_size.y;
    }
    else if (index == 3) 
    {
        x = 0;
        y = m_size.y;
    }
        
    return sf::Vector2f(x, y);
}