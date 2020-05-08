#pragma once

#include <cstddef>
#include <functional>
#include <optional>

#include <ghc/filesystem.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/System.hpp>

#include "../Data/Preferences.hpp"
#include "../Data/Song.hpp"
#include "../Drawables/BlackFrame.hpp"
#include "../Drawables/ButtonHighlight.hpp"
#include "../Drawables/DensityGraph.hpp"
#include "../Resources/Marker.hpp"
#include "../Resources/LNMarker.hpp"
#include "../Resources/TextureCache.hpp"

namespace Resources {

    class MarkerPanel;

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

    // Holds everything that needs to be shared between screens and down the class hierarchy
    struct SharedResources : public Data::HoldsPreferences {
        SharedResources(Data::Preferences& p);

        Textures::TextureCache covers;
        
        FallbackFont fallback_font;

        Drawables::BlackFrame black_frame;
        Drawables::ButtonHighlight button_highlight;
        
        Drawables::DensityGraphCache density_graphs;

        sf::Color BSC_color = sf::Color{34,216,92};
        sf::Color ADV_color = sf::Color{252,212,32};
        sf::Color EXT_color = sf::Color{234,46,32};
        sf::Color get_chart_color(const std::string& chart);

        Resources::Markers markers;
        Resources::Marker& get_selected_marker();

        Resources::LNMarkers ln_markers;
        Resources::LNMarker& get_selected_ln_marker();
    };

    // Proxy for HoldsPreferences
    struct HoldsSharedResources : public Data::HoldsPreferences {
        HoldsSharedResources(SharedResources& t_resources) : Data::HoldsPreferences(t_resources.preferences), shared(t_resources) {};
        SharedResources& shared;
    };
}