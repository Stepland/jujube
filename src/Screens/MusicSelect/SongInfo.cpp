#include "SongInfo.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "Panel.hpp"

namespace MusicSelect {

    BigCover::BigCover(SharedResources& resources) :
        HoldsSharedResources(resources)
    {
        m_cover_fallback.setSize({get_size(), get_size()});
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
        cover.setScale(get_size()/bounds.width, get_size()/bounds.height);
        auto alpha = static_cast<std::uint8_t>(
            m_seconds_to_alpha.clampedTransform(
                selected_panel->selected_since.getElapsedTime().asSeconds()
            )
        );
        cover.setColor(sf::Color(255, 255, 255, alpha));
        target.draw(cover, states);
    }

    SongInfo::SongInfo(SharedResources& resources) :
        HoldsSharedResources(resources),
        m_big_cover(resources)
    {
        m_big_cover.setOrigin(
            m_big_cover.get_size()*0.5f,
            0.f
        );
        m_big_cover.setPosition(
            get_big_cover_x(),
            get_big_cover_y()
        );
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
                    0.026315789f*get_screen_width()
                )
            };
            auto song_title_bounds = song_title_label.getLocalBounds();
            if (song_title_bounds.width > m_big_cover.get_size()) {
                song_title_label.setScale(m_big_cover.get_size() / song_title_bounds.width, 1.0f);
            }
            song_title_label.setFillColor(sf::Color::White);
            auto cover_pos = m_big_cover.getPosition();
            song_title_label.setPosition(
                get_big_cover_x() - m_big_cover.get_size()/2.f,
                get_big_cover_y() + m_big_cover.get_size() + 0.01f*get_screen_width()
            );
            target.draw(song_title_label);
        }
        auto song_artist = selected_chart->song.artist;
        if (not song_artist.empty()) {
            sf::Text song_artist_label{
                song_artist,
                m_resources.noto_sans_medium,
                static_cast<unsigned int>(
                    0.02f*get_screen_width()
                )
            };
            song_artist_label.setStyle(sf::Text::Italic);
            auto song_artist_bounds = song_artist_label.getLocalBounds();
            if (song_artist_bounds.width > m_big_cover.get_size()) {
                song_artist_label.setScale(m_big_cover.get_size() / song_artist_bounds.width, 1.0f);
            }
            song_artist_label.setFillColor(sf::Color::White);
            song_artist_label.setFillColor(sf::Color::White);
            auto cover_pos = m_big_cover.getPosition();
            song_artist_label.setPosition(
                get_big_cover_x() - m_big_cover.get_size()/2.f,
                get_big_cover_y() + m_big_cover.get_size() + 0.05f*get_screen_width()
            );
            target.draw(song_artist_label);
        }
    }
}
