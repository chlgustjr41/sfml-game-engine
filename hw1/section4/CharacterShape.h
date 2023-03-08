#include <SFML/Graphics.hpp>

#ifndef CHARACTERSHAPE_H
#define CHARACTERSHAPE_H

class CharacterShape : public sf::Shape
{
    public :
        explicit CharacterShape(const unsigned int size = 0, 
                                const sf::Vector2f& position = sf::Vector2f(0.f, 0.f));

        virtual std::size_t getPointCount() const;

        virtual sf::Vector2f getPoint(std::size_t index) const;

    private :
        unsigned int m_size;
};

#endif