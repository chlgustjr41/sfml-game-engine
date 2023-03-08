#include <SFML/Graphics.hpp>
#include "CharacterShape.h"

CharacterShape::CharacterShape(const unsigned int size, const sf::Vector2f& position) : 
m_size(size)
{
    setPosition(position.x, position.y);
    update();
}

std::size_t CharacterShape::getPointCount() const
{
    return 4;
}

sf::Vector2f CharacterShape::getPoint(std::size_t index) const
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
        x = m_size;
        y = 0;
    }
    else if (index == 2) 
    {
        x = m_size;
        y = m_size;
    }
    else if (index == 3) 
    {
        x = 0;
        y = m_size;
    }
        
    return sf::Vector2f(x, y);
}