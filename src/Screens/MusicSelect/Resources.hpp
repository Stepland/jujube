#pragma once

#include <SFML/Graphics.hpp>

#include "../../Resources/CoverAtlas.hpp"

namespace MusicSelect {
    struct Resources {
        Resources();
        Textures::CoverAltas cover_previews;
        sf::Font noto_sans_medium;
    };
}