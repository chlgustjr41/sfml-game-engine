#include <SFML/Graphics.hpp>

class MovingPlatformShape : public sf::Shape
{
    public :
        explicit MovingPlatformShape(const sf::Vector2f& size = sf::Vector2f(0.f, 0.f), 
                                const sf::Vector2f& position = sf::Vector2f(0.f, 0.f));

        sf::Vector2f getSize() const;

        virtual std::size_t getPointCount() const;

        virtual sf::Vector2f getPoint(std::size_t index) const;

    private :
        sf::Vector2f m_size;
};