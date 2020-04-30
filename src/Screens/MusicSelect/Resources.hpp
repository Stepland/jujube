#pragma once

#include "../../Resources/SharedResources.hpp"
#include "MusicPreview.hpp"

namespace MusicSelect {
    
    class SelectablePanel;
    class MarkerPanel;
    class OptionPage;

    struct ScreenResources : Resources::HoldsSharedResources {
        ScreenResources(Resources::SharedResources& r) : Resources::HoldsSharedResources(r) {}

        std::optional<Resources::Timed<SelectablePanel>> selected_panel;
        std::string get_last_selected_difficulty();
        std::optional<std::string> get_selected_difficulty();
        std::optional<std::reference_wrapper<const Data::Song>> get_selected_song();

        MusicPreview music_preview;

        std::vector<std::reference_wrapper<OptionPage>> options_state;

        std::optional<Resources::Timed<MarkerPanel>> selected_marker;
    };

    // Proxy for HoldsPreferences
    struct HoldsResources : Resources::HoldsSharedResources {
        HoldsResources(ScreenResources& t_resources) : Resources::HoldsSharedResources(t_resources.shared), resources(t_resources) {};
        ScreenResources& resources;
    };
}