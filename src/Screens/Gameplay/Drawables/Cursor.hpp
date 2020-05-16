#pragma once

#include <array>

#include <SFML/Graphics.hpp>

namespace Drawables {
    // Small thingy that scrolls atop the graded density graph to indicate how far we are in the song
    class Cursor : public sf::Drawable, public sf::Transformable {
    public:
        Cursor();
        sf::FloatRect getLocalBounds() const;
        sf::FloatRect getGlobalBounds() const;
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        std::array<sf::Vertex, 4> m_vertex_array;
    };
}