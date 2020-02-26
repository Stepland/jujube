#pragma once

#include <cstddef>
#include <optional>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/System.hpp>

#include "../../Resources/TextureCache.hpp"
#include "../../Data/Preferences.hpp"
#include "../../Data/Song.hpp"

#include "DensityGraph.hpp"
#include "MusicPreview.hpp"

namespace MusicSelect {

    class SelectablePanel;

    struct TimedSelectedPanel {
        TimedSelectedPanel(SelectablePanel& s) : panel(s), first_click(), last_click() {};
        SelectablePanel& panel;
        sf::Clock first_click;
        sf::Clock last_click;
        bool is_first_click = true;
    };

    struct FallbackFont {
        FallbackFont();
        sf::Font light;
        sf::Font medium;
        sf::Font black;
    };

    // Holds everything that needs to be shared by all levels of the class hierarchy
    struct SharedResources : public Data::HoldsPreferences {
        SharedResources(Data::Preferences& p);

        Textures::TextureCache covers;
        sf::Texture fallback_cover;
        
        FallbackFont fallback_font;

        MusicSelect::DensityGraphCache density_graphs;
        
        std::optional<TimedSelectedPanel> selected_panel;
        std::string get_last_selected_difficulty();
        std::optional<std::string> get_selected_difficulty();
        std::optional<std::reference_wrapper<const Data::Song>> get_selected_song();

        sf::Color BSC_color = sf::Color{34,216,92};
        sf::Color ADV_color = sf::Color{252,212,32};
        sf::Color EXT_color = sf::Color{234,46,32};
        sf::Color get_chart_color(const std::string& chart);

        MusicPreview music_preview;
    };

    // Proxy for HoldsPreferences
    struct HoldsSharedResources : public Data::HoldsPreferences {
        HoldsSharedResources(SharedResources& resources) : Data::HoldsPreferences(resources.m_preferences), m_resources(resources) {};
    protected:
        SharedResources& m_resources;
    };
}