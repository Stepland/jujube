#include "Resources.hpp"


MusicSelect::Resources::Resources() {
    if (not noto_sans_medium.loadFromFile("assets/fonts/NotoSans-Medium.ttf")) {
        throw std::runtime_error("Unable to load assets/fonts/NotoSans-Medium.ttf");
    }    
}
