#pragma once

#include <SFML/Window.hpp>
#include "../Model/MusicList.hpp"
#include "../Model/Chart.hpp"

namespace Screen {
    class MusicSelect {
        MusicList music_list;
    public:
        MusicSelect();
        Chart& display(sf::Window& window) const;
    };
};
