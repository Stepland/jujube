#include "SharedResources.hpp"

#include <iostream>

#include "../../Toolkit/HSL.hpp"

#include "Panel.hpp"

namespace MusicSelect {
    SharedResources::SharedResources(Data::Preferences& p) :
        preferences(p),
        covers(),
        fallback_cover(),
        noto_sans_medium()
    {
        if (not noto_sans_medium.loadFromFile("assets/fonts/NotoSans-Medium.ttf")) {
            throw std::runtime_error("Unable to load assets/fonts/NotoSans-Medium.ttf");
        }
        if (not fallback_cover.loadFromFile("assets/textures/fallback_cover.png")) {
            throw std::runtime_error("Unable to load assets/textures/fallback_cover.png");
        }
        std::cout << "Loaded MusicSelect::SharedResources" << std::endl;
    }

    std::string SharedResources::get_last_selected_difficulty() {
        return get_selected_difficulty().value_or("BSC");
    }

    std::optional<std::string> SharedResources::get_selected_difficulty() {
        if (not selected_panel.has_value()) {
            return {};
        }
        auto chart_selection = selected_panel->panel.get_selected_difficulty();
        if (not chart_selection.has_value()) {
            return {};
        }
        return chart_selection->difficulty;
    }

    std::optional<std::reference_wrapper<const Data::Song>> MusicSelect::SharedResources::get_selected_song() {
        if (not selected_panel.has_value()) {
            return {};
        }
        auto chart_selection = selected_panel->panel.get_selected_difficulty();
        if (not chart_selection.has_value()) {
            return {};
        }
        return chart_selection->song;
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
}
