#include "SongInfo.hpp"

#include <cmath>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "../../Toolkit/NormalizedOrigin.hpp"

#include "Panel.hpp"

namespace MusicSelect {

    BigCover::BigCover(SharedResources& resources) :
        HoldsSharedResources(resources)
    {
        m_cover_fallback.setFillColor(sf::Color::Transparent);
        m_cover_fallback.setOutlineThickness(1.f);
        m_cover_fallback.setOutlineColor(sf::Color::White);
    }

    void BigCover::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        m_cover_fallback.setSize({get_size(), get_size()});
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

    SongInfo::SongInfo(SharedResources& resources) : HoldsSharedResources(resources), m_big_cover(resources) {

    }

    void SongInfo::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        m_big_cover.setOrigin(m_big_cover.get_size()*0.5f, 0.f);
        m_big_cover.setPosition(get_big_cover_x(), get_big_cover_y());
        target.draw(m_big_cover, states);
        draw_song_title(target, states);
        draw_big_level(target, states);
    }

    void SongInfo::draw_song_title(sf::RenderTarget& target, sf::RenderStates states) const {
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
            target.draw(song_title_label, states);
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
            target.draw(song_artist_label, states);
        }
    }
    void SongInfo::draw_big_level(sf::RenderTarget& target, sf::RenderStates states) const {
        auto selected_panel = m_resources.selected_panel;
        if (not selected_panel.has_value()) {
            return;
        }
        auto selected_chart = selected_panel->panel.get_selected_chart();
        if (not selected_chart.has_value()) {
            return;
        }
        sf::Text level_label{
            "LEVEL",
            m_resources.noto_sans_medium,
            static_cast<unsigned int>(12.f/768.f*get_screen_width())
        };
        Toolkit::set_origin_normalized(level_label, 0.5f, 0.f);
        level_label.setPosition(get_big_level_x(), get_big_level_y());
        level_label.setFillColor(sf::Color::White);
        target.draw(level_label, states);
        
        sf::Text level_number_label{
            std::to_string(selected_chart->song.chart_levels.at(selected_chart->chart)),
            m_resources.noto_sans_medium,
            static_cast<unsigned int>(130.f/768.f*get_screen_width())
        };
        Toolkit::set_origin_normalized(level_number_label, 0.5f, 0.f);
        level_number_label.setPosition(get_big_level_x(), get_big_level_y()+(30.f/768.f*get_screen_width()));
        level_number_label.setFillColor(sf::Color::White);
        target.draw(level_number_label, states);

        std::string full_chart_name = selected_chart->chart;
        if (selected_chart->chart == "BSC") {
            full_chart_name = "BASIC";
        } else if (selected_chart->chart == "ADV") {
            full_chart_name = "ADVANCED";
        } else if (selected_chart->chart == "EXT") {
            full_chart_name = "EXTREME";
        }

        sf::Text chart_label{
            full_chart_name,
            m_resources.noto_sans_medium,
            static_cast<unsigned int>(20.f/768.f*get_screen_width())
        };
        Toolkit::set_origin_normalized_no_position(chart_label, 0.5f, 0.f);
        chart_label.setPosition(get_big_level_x(), get_big_level_y()+(145.f/768.f*get_screen_width()));
        chart_label.setFillColor(m_resources.get_chart_color(selected_chart->chart));
        target.draw(chart_label, states);
    }
}
