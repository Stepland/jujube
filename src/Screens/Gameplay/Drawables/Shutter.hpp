#pragma once

#include <SFML/Graphics.hpp>

#include "../../../Toolkit/AffineTransform.hpp"

namespace Drawables {
    // 1080x1080 drawable 
    class Shutter : public sf::Drawable, public sf::Transformable {
    public:
        Shutter();
        void update(int shutter_value);
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        sf::RectangleShape color_background;
        Toolkit::AffineTransform<float> shutter_to_alpha;
    };
}