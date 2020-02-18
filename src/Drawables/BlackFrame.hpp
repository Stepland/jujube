#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include "../Data/Preferences.hpp"

namespace Drawables {
    class BlackFrame : public sf::Drawable, public sf::Transformable, public Data::HoldsPreferences {
    public:
        BlackFrame(Data::Preferences& t_preferences) :
            Data::HoldsPreferences(t_preferences),
            m_vertex_array()
        {};
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        mutable std::vector<sf::Vertex> m_vertex_array;
    };

    std::array<sf::Vertex, 4> make_solid_quad(const sf::FloatRect& rect, const sf::Color& color);
}
