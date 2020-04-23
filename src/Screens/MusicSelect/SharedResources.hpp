#pragma once

#include <cstddef>
#include <functional>
#include <optional>

#include <ghc/filesystem.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/System.hpp>

#include "../../Data/Preferences.hpp"
#include "../../Data/Song.hpp"
#include "../../Drawables/ButtonHighlight.hpp"
#include "../../Resources/Marker.hpp"
#include "../../Resources/TextureCache.hpp"

#include "DensityGraph.hpp"
#include "MusicPreview.hpp"

namespace MusicSelect {

    class SelectablePanel;
    class MarkerPanel;
    class OptionPage;

    template<class Object>
    struct Timed {
        Timed(Object& t_obj) : obj(t_obj), first_click(), last_click() {};
        Object& obj;
        sf::Clock first_click;
        sf::Clock last_click;
        bool is_first_click = true;
    };

    struct FallbackFont {
        FallbackFont(const ghc::filesystem::path& jujube_path);
        sf::Font light;
        sf::Font medium;
        sf::Font black;
    };

    // Holds everything that needs to be shared by all levels of the class hierarchy
    struct SharedResources : public Data::HoldsPreferences {
        SharedResources(Data::Preferences& p);

        Textures::TextureCache covers;
        
        FallbackFont fallback_font;

        MusicSelect::DensityGraphCache density_graphs;
        
        std::optional<Timed<SelectablePanel>> selected_panel;
        std::string get_last_selected_difficulty();
        std::optional<std::string> get_selected_difficulty();
        std::optional<std::reference_wrapper<const Data::Song>> get_selected_song();

        sf::Color BSC_color = sf::Color{34,216,92};
        sf::Color ADV_color = sf::Color{252,212,32};
        sf::Color EXT_color = sf::Color{234,46,32};
        sf::Color get_chart_color(const std::string& chart);

        MusicPreview music_preview;

        std::vector<std::reference_wrapper<OptionPage>> options_state;

        Resources::Markers markers;
        std::optional<Timed<MarkerPanel>> selected_marker;

        Drawables::ButtonHighlight button_highlight;
    };

    // Proxy for HoldsPreferences
    struct HoldsSharedResources : public Data::HoldsPreferences {
        HoldsSharedResources(SharedResources& t_resources) : Data::HoldsPreferences(t_resources.preferences), resources(t_resources) {};
    protected:
        SharedResources& resources;
    };
}