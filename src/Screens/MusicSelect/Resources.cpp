#include "Resources.hpp"

#include <iostream>

MusicSelect::Resources::Resources() :
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
}
