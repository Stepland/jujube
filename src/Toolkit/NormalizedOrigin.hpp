#pragma once

#include <SFML/Graphics.hpp>

namespace Toolkit {
    void setNormOrigin(sf::Shape& s, float x, float y) {
        auto bounds = s.getGlobalBounds();
        s.setOrigin(bounds.left+x*bounds.width, bounds.top+y*bounds.height);
    };
    void setNormOrigin(sf::Sprite& s, float x, float y) {
        auto bounds = s.getGlobalBounds();
        s.setOrigin(bounds.left+x*bounds.width, bounds.top+y*bounds.height);
    }
    void setNormOrigin(sf::Text& t, float x, float y) {
        auto bounds = t.getGlobalBounds();
        t.setOrigin(bounds.left+x*bounds.width, bounds.top+y*bounds.height);
    }
}