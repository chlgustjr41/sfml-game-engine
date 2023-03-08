#include <SFML/Graphics.hpp>

class PlatformShape : public sf::Shape
{
    public :
        explicit PlatformShape(const sf::Vector2f& size = sf::Vector2f(0.f, 0.f), 
                                const sf::Vector2f& position = sf::Vector2f(0.f, 0.f));

        virtual std::size_t getPointCount() const;

        virtual sf::Vector2f getPoint(std::size_t index) const;

    private :
        sf::Vector2f m_size;
};