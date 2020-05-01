#include "SharedResources.hpp"

#include <iostream>

#include "../Toolkit/HSL.hpp"

namespace Resources {

    FallbackFont::FallbackFont(const ghc::filesystem::path& jujube_path) :
        light(),
        medium(),
        black()
    {
        auto fallback_font_folder = jujube_path/"assets"/"fonts"/"M_PLUS_Rounded_1c";
        if (not light.loadFromFile(fallback_font_folder/"MPLUSRounded1c-Light.ttf")) {
            throw std::runtime_error("Unable to load MPLUSRounded1c-Light.ttf");
        }
        if (not medium.loadFromFile(fallback_font_folder/"RoundedMplus1c-Medium+NotoSansSymbols+Symbola.ttf")) {
            throw std::runtime_error("Unable to load RoundedMplus1c-Medium+NotoSansSymbols+Symbola.ttf");
        }
        if (not black.loadFromFile(fallback_font_folder/"MPLUSRounded1c-Black.ttf")) {
            throw std::runtime_error("Unable to load MPLUSRounded1c-Black.ttf");
        }
    }
    
    SharedResources::SharedResources(Data::Preferences& p) :
        Data::HoldsPreferences(p),
        fallback_font(p.jujube_path),
        black_frame(p),
        button_highlight(p),
        markers(Resources::load_markers(p.jujube_path))
    {
        covers.reserve(256);
        std::cout << "Loaded MusicSelect::SharedResources" << std::endl;
    }

    sf::Color SharedResources::get_chart_color(const std::string& chart) {
        if (chart == "BSC") {
            return BSC_color;
        } else if (chart == "ADV") {
            return ADV_color;
        } else if (chart == "EXT") {
            return EXT_color;
        } else {
            return Toolkit::HSL(
                static_cast<int>(std::hash<std::string>{}(chart)),
                83,
                49
            ).TurnToRGB();
        }
    }

    Resources::Marker& SharedResources::get_selected_marker() {
        return markers.at(preferences.options.marker);
    }
}
