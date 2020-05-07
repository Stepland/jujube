#include "SFMLHelpers.hpp"

namespace Toolkit {
    void set_size_from_local_bounds(sf::Sprite& s, float x, float y) {
        auto const bounds = s.getLocalBounds();
        s.setScale(x/bounds.width,y/bounds.height);
    }
}
