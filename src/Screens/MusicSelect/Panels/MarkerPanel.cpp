#include "MarkerPanel.hpp"

#include <cmath>

namespace MusicSelect {
    MarkerPanel::MarkerPanel(SharedResources& resources, const Resources::Marker& marker) :
        Panel(resources),
        m_marker(marker)
    {
        if (m_resources.m_preferences.options.marker == m_marker.m_metadata.name) {
            select();
        }
    }

    void MarkerPanel::click(Ribbon&, const Data::Button&) {
        if (selected) {
            m_resources.selected_marker->last_click.restart();
            m_resources.selected_marker->is_first_click = false;
        } else {
            select();
        }
    }

    void MarkerPanel::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        float animation_time = 0.f;
        if (selected) {
            animation_time = std::fmod(m_resources.selected_marker->last_click.getElapsedTime().asSeconds(), 2.f) - 1.f;
        }
        auto sprite = m_marker.get_sprite(Resources::MarkerAnimation::APPROACH, animation_time);
        if (sprite) {
            auto bounds = sprite->getGlobalBounds();
            sprite->setScale(get_panel_size()/bounds.width, get_panel_size()/bounds.height);
            target.draw(*sprite, states);
        }
    }

    void MarkerPanel::select() {
        if (m_resources.selected_marker) {
            m_resources.selected_marker->obj.unselect();
        }
        m_resources.selected_marker.emplace(*this);
        m_resources.m_preferences.options.marker = m_marker.m_metadata.name;
        this->selected = true;
    }

    void MarkerPanel::unselect() {
        this->selected = false;
    }
}
