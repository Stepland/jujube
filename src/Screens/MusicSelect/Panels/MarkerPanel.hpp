#pragma once

#include <optional>

#include <SFML/System.hpp>

#include "../../../Resources/Marker.hpp"
#include "Panel.hpp"

namespace MusicSelect {
    class MarkerPanel final : public Panel {
    public:
        MarkerPanel(SharedResources& resources, const Resources::Marker& marker);
        void click(Ribbon&, const Data::Button&) override;
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        void select();
        void unselect();
        const Resources::Marker& m_marker;
        bool selected = false;
    };
}