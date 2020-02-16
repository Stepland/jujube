#pragma once

#include <SFML/Graphics.hpp>

namespace Toolkit {
    template<class T>
    void set_origin_normalized(T& s, float x, float y) {
        auto bounds = s.getGlobalBounds();
        s.setOrigin(bounds.left+x*bounds.width, bounds.top+y*bounds.height);
    }
    
    template<class T>
    void set_origin_normalized_no_position(T& s, float x, float y) {
        auto bounds = s.getGlobalBounds();
        s.setOrigin(x*bounds.width, y*bounds.height);
    }
}