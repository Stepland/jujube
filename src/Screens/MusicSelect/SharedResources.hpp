#pragma once

#include <cstddef>
#include <optional>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "../../Resources/Autoloader.hpp"
#include "../../Data/SongList.hpp"

#include "Panel.hpp"

namespace MusicSelect {

    class SelectablePanel;

    struct TimedSelectedPanel {
        TimedSelectedPanel(SelectablePanel& s) : panel(s), selected_since() {};
        SelectablePanel& panel;
        sf::Clock selected_since;
    };

    struct SharedResources {
        SharedResources();
        Textures::Autoloader covers;
        sf::Texture fallback_cover;
        sf::Font noto_sans_medium;
        std::optional<TimedSelectedPanel> selected_panel;
    };
}