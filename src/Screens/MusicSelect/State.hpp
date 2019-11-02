#pragma once

#include <array>
#include <functional>
#include <optional>
#include <vector>

#include "../Data/SongList.hpp"
#include "Panel.hpp"
#include "Ribbon.hpp"


namespace MusicSelect {
    // Holds everything related to the displaying state of the MusicSelect screen
    struct State {
        State(const Data::SongList& song_list);
        // The ribbon is the moving part or the music select screen
        Ribbon ribbon;
        unsigned int ribbon_position;
        std::optional<std::reference_wrapper<SongPanel>> selected_panel;
    };
}