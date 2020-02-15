#include "SongInfo.hpp"

#include <SFML/Graphics.hpp>

namespace MusicSelect {

    BigCover::BigCover(SharedResources& resources, const float& size) :
        m_resources(resources),
        m_size(size),
        m_cover_fallback({size,size})
    {
        m_cover_fallback.setFillColor(sf::Color::Transparent);
        m_cover_fallback.setOutlineThickness(1.f);
        m_cover_fallback.setOutlineColor(sf::Color::White);
    }

    void BigCover::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        target.draw(m_cover_fallback, states);
        auto selected_panel = m_resources.selected_panel;
        if (not selected_panel.has_value()) {
            return;
        }
        auto selected_chart = selected_panel->panel.get_selected_chart();
        if (not selected_chart.has_value()) {
            return;
        }
        auto cover_path = selected_chart->song.full_cover_path();
        if (not cover_path.has_value()) {
            return;
        }
        auto cover_texture = m_resources.covers.async_get(*cover_path);
        if (not cover_texture.has_value()) {
            return;
        }
        sf::Sprite cover{*(cover_texture->texture)};
        auto bounds = cover.getGlobalBounds();
        cover.setScale(m_size/bounds.width, m_size/bounds.height);
        auto alpha = static_cast<std::uint8_t>(
            m_seconds_to_alpha.clampedTransform(
                selected_panel->selected_since.getElapsedTime().asSeconds()
            )
        );
        cover.setColor(sf::Color(255, 255, 255, alpha));
        target.draw(cover, states);
    }

    SongInfo::SongInfo(SharedResources& resources, const float& width, const float& height) :
        m_resources(resources),
        m_width(width),
        m_height(height),
        m_big_cover(resources, width*0.42f)
    {
        m_big_cover.setOrigin(width*0.42f*0.5f, 0.f);
        m_big_cover.setPosition(width*0.5f, width*0.017f);
    }

    void SongInfo::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        target.draw(m_big_cover, states);
        draw_song_title(target);
    }

    void SongInfo::draw_song_title(sf::RenderTarget& target) const {
        auto selected_panel = m_resources.selected_panel;
        if (not selected_panel.has_value()) {
            return;
        }
        auto selected_chart = selected_panel->panel.get_selected_chart();
        if (not selected_chart.has_value()) {
            return;
        }
        auto song_title = selected_chart->song.title;
        if (not song_title.empty()) {
            sf::Text song_title_label{
                song_title,
                m_resources.noto_sans_medium,
                static_cast<unsigned int>(
                    0.026315789f*m_width
                )
            };
            auto song_title_bounds = song_title_label.getLocalBounds();
            if (song_title_bounds.width > 0.42f * m_width) {
                song_title_label.setScale(0.42f * m_width / song_title_bounds.width, 1.0f);
            }
            song_title_label.setFillColor(sf::Color::White);
            auto cover_pos = m_big_cover.getPosition();
            song_title_label.setPosition(
                m_width*(0.5f - (0.42f/2.f)),
                m_width*(0.017f + 0.42f + 0.01f)
            );
            target.draw(song_title_label);
        }
        auto song_artist = selected_chart->song.artist;
        if (not song_artist.empty()) {
            sf::Text song_artist_label{
                song_artist,
                m_resources.noto_sans_medium,
                static_cast<unsigned int>(
                    0.02f*m_width
                )
            };
            song_artist_label.setStyle(sf::Text::Italic);
            auto song_artist_bounds = song_artist_label.getLocalBounds();
            if (song_artist_bounds.width > 0.42f * m_width) {
                song_artist_label.setScale(0.42f * m_width / song_artist_bounds.width, 1.0f);
            }
            song_artist_label.setFillColor(sf::Color::White);
            song_artist_label.setFillColor(sf::Color::White);
            auto cover_pos = m_big_cover.getPosition();
            song_artist_label.setPosition(
                m_width*(0.5f - (0.42f/2.f)),
                m_width*(0.017f + 0.42f + 0.01f + 0.04f)
            );
            target.draw(song_artist_label);
        }
    };

}
