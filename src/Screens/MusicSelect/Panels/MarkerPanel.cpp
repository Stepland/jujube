#include "MarkerPanel.hpp"

#include <cmath>

namespace MusicSelect {
    MarkerPanel::MarkerPanel(ScreenResources& t_resources, const Resources::Marker& t_marker) :
        Panel(t_resources),
        marker(t_marker)
    {
        if (shared.preferences.options.marker == marker.name) {
            select();
        }
    }

    void MarkerPanel::click(Ribbon&, const Input::Button&) {
        if (selected) {
            resources.selected_marker->last_click.restart();
            resources.selected_marker->is_first_click = false;
        } else {
            select();
        }
    }

    void MarkerPanel::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        float animation_time = 0.f;
        if (selected) {
            animation_time = std::fmod(resources.selected_marker->last_click.getElapsedTime().asSeconds(), 2.f) - 1.f;
        }
        auto sprite = marker.get_sprite(Resources::MarkerAnimation::APPROACH, animation_time);
        if (sprite) {
            auto bounds = sprite->getGlobalBounds();
            sprite->setScale(get_panel_size()/bounds.width, get_panel_size()/bounds.height);
            target.draw(*sprite, states);
        }
    }

    void MarkerPanel::select() {
        if (resources.selected_marker) {
            resources.selected_marker->obj.unselect();
        }
        resources.selected_marker.emplace(*this);
        preferences.options.marker = marker.name;
        this->selected = true;
    }

    void MarkerPanel::unselect() {
        this->selected = false;
    }
}
