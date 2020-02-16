#include "SharedResources.hpp"

#include <iostream>

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

    std::string SharedResources::get_last_selected_chart() {
        return get_selected_chart().value_or("BSC");
    }

    std::optional<std::string> SharedResources::get_selected_chart() {
        if (not selected_panel.has_value()) {
            return {};
        }
        auto chart_selection = selected_panel->panel.get_selected_chart();
        if (not chart_selection.has_value()) {
            return {};
        }
        return chart_selection->chart;
    }

    std::optional<std::reference_wrapper<const Data::Song>> MusicSelect::SharedResources::get_selected_song() {
        if (not selected_panel.has_value()) {
            return {};
        }
        auto chart_selection = selected_panel->panel.get_selected_chart();
        if (not chart_selection.has_value()) {
            return {};
        }
        return chart_selection->song;
    }
}
