#include "Cursor.hpp"

namespace Drawables {
    Cursor::Cursor() {
        m_vertex_array = {
            sf::Vertex{
                sf::Vector2f(1.f, 0.f),
                sf::Color::White
            },
            sf::Vertex{
                sf::Vector2f(1.f, 1.f),
                sf::Color::White
            },
            sf::Vertex{
                sf::Vector2f(0.f, 1.f),
                sf::Color::Transparent
            },
            sf::Vertex{
                sf::Vector2f(0.f, 0.f),
                sf::Color::Transparent
            }
        };
    }

    sf::FloatRect Cursor::getLocalBounds() const {
        return sf::FloatRect{{0.f,0.f}, {1.f,1.f}};
    }

    sf::FloatRect Cursor::getGlobalBounds() const {
        return getTransform().transformRect(getLocalBounds());
    }

    void Cursor::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        target.draw(m_vertex_array.data(), m_vertex_array.size(), sf::Quads, states);
    }
}
