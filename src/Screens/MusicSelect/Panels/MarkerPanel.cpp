#include "MarkerPanel.hpp"

#include <cmath>

namespace MusicSelect {
    MarkerPanel::MarkerPanel(SharedResources& resources, const Resources::Marker& marker) :
        Panel(resources),
        m_marker(marker)
    {
        if (m_resources.m_preferences.options.marker == marker.m_metadata.name) {
            selected_since.emplace();
        }
    };

    void MarkerPanel::click(Ribbon&, const Data::Button&) {
        if (not selected_since) {
            selected_since.emplace();
            m_resources.m_preferences.options.marker = m_marker.m_metadata.name;
        }
    };

    void MarkerPanel::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        float animation_time = 0.f;
        if (selected_since) {
            auto animation_time = std::fmodf(selected_since->getElapsedTime().asSeconds(), 2.f) - 1.f;
        }
        auto sprite = m_marker.get_sprite(Resources::MarkerAnimation::APPROACH, 0.f);
        if (sprite) {
            target.draw(*sprite, states);
        }
    };
}
