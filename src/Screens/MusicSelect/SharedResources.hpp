#pragma once

#include <cstddef>
#include <optional>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "../../Resources/Autoloader.hpp"
#include "../../Data/Preferences.hpp"
#include "../../Data/SongList.hpp"

namespace MusicSelect {

    class SelectablePanel;

    struct TimedSelectedPanel {
        TimedSelectedPanel(SelectablePanel& s) : panel(s), first_click(), last_click() {};
        SelectablePanel& panel;
        sf::Clock first_click;
        sf::Clock last_click;
        bool is_first_click = true;
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
        sf::Color get_chart_color(const std::string& chart);
    };

    struct HoldsSharedResources {
        HoldsSharedResources(SharedResources& resources) : m_resources(resources) {};
        float get_screen_width() const {return static_cast<float>(m_resources.preferences.screen.width);};
        float get_screen_height() const {return static_cast<float>(m_resources.preferences.screen.height);};
        float get_panel_size() const {return m_resources.preferences.layout.panel_size*get_screen_width();};
        float get_panel_spacing() const {return m_resources.preferences.layout.panel_spacing*get_screen_width();};
        float get_panel_step() const {return m_resources.preferences.layout.panel_step()*get_screen_width();};
        float get_big_cover_x() const {return m_resources.preferences.layout.big_cover_x*get_screen_width();};
        float get_big_cover_y() const {return m_resources.preferences.layout.big_cover_y*get_screen_width();};
        float get_big_level_x() const {return m_resources.preferences.layout.big_level_x*get_screen_width();};
        float get_big_level_y() const {return m_resources.preferences.layout.big_level_y*get_screen_width();};
    protected:
        SharedResources& m_resources;
    };
}