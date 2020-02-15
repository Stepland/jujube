#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "../../Toolkit/AffineTransform.hpp"

#include "SharedResources.hpp"

namespace MusicSelect {

    class BigCover : public sf::Drawable, public sf::Transformable {
    public:
        BigCover(SharedResources& resources, const float& size);
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        SharedResources& m_resources;
        float m_size;
        sf::RectangleShape m_cover_fallback;
        const Toolkit::AffineTransform<float> m_seconds_to_alpha{0.0f, 0.3f, 0.f, 255.f};
    };

    // Displays the song info on the top part of the screen
    class SongInfo : public sf::Drawable, public sf::Transformable {
    public:
        SongInfo(SharedResources& resources, const float& width, const float& height);
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        void draw_song_title(sf::RenderTarget& target) const;
        SharedResources& m_resources;
        const float& m_width;
        const float& m_height;
        BigCover m_big_cover;
    };
}
