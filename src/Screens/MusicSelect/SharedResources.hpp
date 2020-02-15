#pragma once

#include <cstddef>
#include <optional>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "../../Resources/Autoloader.hpp"
#include "../../Data/Preferences.hpp"
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
        SharedResources(Data::Preferences& p);
        
        Data::Preferences& preferences;

        Textures::Autoloader covers;
        sf::Texture fallback_cover;
        
        sf::Font noto_sans_medium;
        
        std::optional<TimedSelectedPanel> selected_panel;
        std::string get_last_selected_chart();
        std::optional<std::string> get_selected_chart();
        std::optional<std::reference_wrapper<const Data::Song>> get_selected_song();

        sf::Color BSC_color = sf::Color{34,216,92};
        sf::Color ADV_color = sf::Color{252,212,32};
        sf::Color EXT_color = sf::Color{234,46,32};
    };
}