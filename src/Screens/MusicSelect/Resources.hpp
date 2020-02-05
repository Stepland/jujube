#pragma once

#include <SFML/Graphics.hpp>

#include "../../Resources/Autoloader.hpp"

namespace MusicSelect {
    struct Resources {
        Resources();
        Textures::Autoloader covers;
        sf::Texture fallback_cover;
        sf::Font noto_sans_medium;
    };
}