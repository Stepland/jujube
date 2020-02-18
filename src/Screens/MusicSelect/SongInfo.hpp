#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "../../Toolkit/AffineTransform.hpp"

#include "SharedResources.hpp"

namespace MusicSelect {

    class BigCover : public sf::Drawable, public sf::Transformable, public HoldsSharedResources {
    public:
        BigCover(SharedResources& resources);
        float get_size() const {return m_preferences.layout.big_cover_size*get_screen_width();};
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        mutable sf::RectangleShape m_cover_fallback;
        const Toolkit::AffineTransform<float> m_seconds_to_alpha{0.0f, 0.3f, 0.f, 255.f};
    };

    // Displays the song info on the top part of the screen
    class SongInfo : public sf::Drawable, public sf::Transformable, public HoldsSharedResources {
    public:
        SongInfo(SharedResources& resources);
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        void draw_song_title(sf::RenderTarget& target, sf::RenderStates states) const;
        void draw_big_level(sf::RenderTarget& target, sf::RenderStates states) const;
        void draw_chart_list(sf::RenderTarget& target, sf::RenderStates states) const;
        void draw_density_graph(sf::RenderTarget& target, sf::RenderStates states) const;
        mutable BigCover m_big_cover;
        const Toolkit::AffineTransform<float> m_seconds_to_badge_anim{0.f, 0.15f, 0.f, 1.f};
    };
}
