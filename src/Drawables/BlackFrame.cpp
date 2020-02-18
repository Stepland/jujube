#include "BlackFrame.hpp"

namespace Drawables {
    void BlackFrame::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        m_vertex_array.clear();
        
        auto top_bar = make_solid_quad(
            sf::FloatRect(
                sf::Vector2f(0.f, get_upper_part_height()),
                sf::Vector2f(get_screen_width(), get_ribbon_y()-get_upper_part_height())
            ),
            sf::Color::Black
        );
        m_vertex_array.insert(m_vertex_array.end(), top_bar.begin(), top_bar.end());
        
        // Horizontal bars
        for (int i = 0; i < 4; i++) {
            sf::Vector2f offset{0.f, get_ribbon_y()+get_panel_size()+i*get_panel_step()};
            auto bar = make_solid_quad(
                sf::FloatRect(offset, {get_screen_width(), get_panel_spacing()}),
                sf::Color::Black
            );
            m_vertex_array.insert(m_vertex_array.end(), bar.begin(), bar.end());
        }
        
        // Vertical bars
        for (int i = -1; i < 4; i++) {
            sf::Vector2f offset = {
                get_ribbon_x()+get_panel_size()+i*get_panel_step(),
                get_ribbon_y()-8.f/768.f*get_screen_height()
            };
            auto bar = make_solid_quad(
                sf::FloatRect(offset, {get_panel_spacing(), get_screen_width()}),
                sf::Color::Black
            );
            m_vertex_array.insert(m_vertex_array.end(), bar.begin(), bar.end());
        }
        target.draw(m_vertex_array.data(), m_vertex_array.size(), sf::Quads);
    }

    std::array<sf::Vertex, 4> make_solid_quad(const sf::FloatRect& rect, const sf::Color& color) {
        return {
            sf::Vertex(sf::Vector2f(rect.left, rect.top), color),
            sf::Vertex(sf::Vector2f(rect.left+rect.width, rect.top), color),
            sf::Vertex(sf::Vector2f(rect.left+rect.width, rect.top+rect.height), color),
            sf::Vertex(sf::Vector2f(rect.left, rect.top+rect.height), color)
        };
    }
}
