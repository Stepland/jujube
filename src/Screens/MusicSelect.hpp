#pragma once

#include <SFML/Window.hpp>
#include "../Data/SongList.hpp"
#include "../Data/Chart.hpp"

namespace Screen {
    
    // The music select screen is created only once and loads a cache of available songs
    // in the music_list attribute
    class MusicSelect {
        Data::SongList music_list;
    public:
        MusicSelect() = default;
        Chart& select_chart(sf::Window& window) const;
    };
};
