#pragma once

#include <unordered_map>

#include <SFML/Graphics.hpp>

#include "../../Resources/CoverAtlas.hpp"

namespace MusicSelect {
    // Holds the fonts, graphics and sounds needed by all of the MusicSelect Screen
    struct Resources {
        Textures::CoverAltas cover_previews;
        std::unordered_map<std::string, sf::Font> fonts;
    };
}
