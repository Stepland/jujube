#include "Resources.hpp"

#include "Panels/Panel.hpp"

namespace MusicSelect {
    std::string ScreenResources::get_last_selected_difficulty() {
        return get_selected_difficulty().value_or("BSC");
    }

    std::optional<std::string> ScreenResources::get_selected_difficulty() {
        if (not selected_panel.has_value()) {
            return {};
        }
        auto chart_selection = selected_panel->obj.get_selected_difficulty();
        if (not chart_selection.has_value()) {
            return {};
        }
        return chart_selection->difficulty;
    }

    std::optional<std::reference_wrapper<const Data::Song>> ScreenResources::get_selected_song() {
        if (not selected_panel.has_value()) {
            return {};
        }
        auto chart_selection = selected_panel->obj.get_selected_difficulty();
        if (not chart_selection.has_value()) {
            return {};
        }
        return chart_selection->song;
    }
}