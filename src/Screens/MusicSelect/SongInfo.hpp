#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "../../Toolkit/AffineTransform.hpp"

#include "SharedResources.hpp"

namespace MusicSelect {
    // SongInfo displays the song info on the top part of the screen
    class SongInfo : public sf::Drawable, public sf::Transformable {
    public:
        SongInfo(SharedResources& resources, const float& width, const float& height);
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        void draw_cover(sf::RenderTarget& target, sf::RenderStates states) const;
        SharedResources& m_resources;
        const float& m_width;
        const float& m_height;
        sf::RectangleShape m_cover_fallback;
        const Toolkit::AffineTransform<float> m_seconds_to_alpha{0.0f, 0.3f, 0.f, 255.f};
    };
}
