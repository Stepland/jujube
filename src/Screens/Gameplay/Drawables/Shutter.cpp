#include "Shutter.hpp"

namespace Drawables {
    Shutter::Shutter() :
        color_background({1080, 1080}),
        shutter_to_alpha(0.f, 1024.f, 0.f, 255.f)
    {
        color_background.setFillColor(sf::Color::Transparent);
    }

    void Shutter::update(int shutter_value) {
        auto alpha = static_cast<sf::Uint8>(shutter_to_alpha.clampedTransform(static_cast<float>(shutter_value)));
        color_background.setFillColor(
            sf::Color::White * sf::Color(alpha, alpha, alpha, alpha)
        );
    }

    void Shutter::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        states.blendMode = sf::BlendAdd;
        target.draw(color_background, states);
    }
}
