#include "Panel.hpp"

#include <functional>

#include <SFML/Graphics.hpp>

#include "../../Toolkit/HSL.hpp"
#include "../../Toolkit/NormalizedOrigin.hpp"

#include "MusicSelect.hpp"
#include "SharedResources.hpp"

namespace MusicSelect {
    void ColorPanel::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        sf::RectangleShape panel{{m_size*0.9f, m_size*0.9f}};
        panel.setFillColor(m_color);
        states.transform *= getTransform();
        target.draw(panel, states);
    }

    void CategoryPanel::click(Ribbon& ribbon, std::size_t from_button_index) {
        ribbon.move_to_next_category(from_button_index);
    }

    void CategoryPanel::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        sf::RectangleShape red_rectangle;
        red_rectangle.setFillColor(sf::Color::Transparent);
        red_rectangle.setOutlineColor(sf::Color::Red);
        red_rectangle.setOutlineThickness(1.f);

        sf::RectangleShape frame{{m_size*0.9f, m_size*0.9f}};
        frame.setFillColor(sf::Color::Black);
        frame.setOutlineThickness(1.f);
        frame.setOutlineColor(sf::Color::White);
        Toolkit::setNormOrigin(frame, 0.5f, 0.5f);
        frame.setPosition(m_size/2.f, m_size/2.f);
        target.draw(frame, states);

        sf::Text top_text;
        top_text.setFont(m_resources.noto_sans_medium);
        top_text.setString("category");
        top_text.setCharacterSize(50U);
        top_text.setFillColor(sf::Color::White);
        auto bounds = top_text.getLocalBounds();
        top_text.setScale(m_size*0.45f / bounds.width, m_size*0.45f / bounds.width);
        top_text.setPosition(m_size*0.07f, m_size*0.07f);
        target.draw(top_text, states);

        set_to_global_bounds(red_rectangle, top_text);
        target.draw(red_rectangle, states);

        sf::Text label_text;
        label_text.setFont(m_resources.noto_sans_medium);
        label_text.setString(m_label);
        label_text.setCharacterSize(100U);
        label_text.setFillColor(sf::Color::White);
        auto text_bounds = label_text.getLocalBounds();
        Toolkit::setNormOrigin(label_text, 0.5f, 0.5f);
        if (text_bounds.height > text_bounds.width) {
            label_text.setScale(m_size*0.8f/text_bounds.height, m_size*0.8f/text_bounds.height);
        } else {
            label_text.setScale(m_size*0.8f/text_bounds.width, m_size*0.8f/text_bounds.width);
        }
        label_text.setPosition(m_size / 2.f, m_size / 2.f);
        target.draw(label_text, states);

        set_to_global_bounds(red_rectangle, label_text);
        target.draw(red_rectangle, states);
    }

    void SongPanel::click(Ribbon& ribbon, std::size_t from_button_index) {
        if (selected_chart.has_value()) {
            // The song was already selected : look for the next chart in order
            auto it = m_song.chart_levels.upper_bound(*selected_chart);
            if (it != m_song.chart_levels.cend()) {
                selected_chart = it->first;
            } else {
                selected_chart = m_song.chart_levels.cbegin()->first;
            }
        } else {
            // The song was not selected before : unselect the last one
            if (m_resources.selected_panel.has_value()) {
                m_resources.selected_panel->panel.unselect();
            }
            // Look for the first chart with dif greater or equal to the last select one
            // or else select the first chart
            auto it = m_song.chart_levels.lower_bound(ribbon.m_global_chart_dif);
            if (it != m_song.chart_levels.cend()) {
                selected_chart = it->first;
            } else {
                selected_chart = m_song.chart_levels.cbegin()->first;
            }
            m_resources.selected_panel.emplace(TimedSelectedPanel{*this});
        }
    }

    void SongPanel::unselect() {
        selected_chart.reset();
    }

    std::optional<ChartSelection> SongPanel::get_selected_chart() const {
        if (selected_chart) {
            return ChartSelection{m_song, *selected_chart};
        } else {
            return {};
        }
    }

    void SongPanel::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        // We should gray out the panel if the currently selected difficulty doesn't exist for this song
        auto selected_chart = m_resources.get_last_selected_chart();
        bool should_be_grayed_out = m_song.chart_levels.find(selected_chart) == m_song.chart_levels.end();
        if (m_song.cover) {
            auto loaded_texture = m_resources.covers.async_get(m_song.folder/m_song.cover.value());
            if (loaded_texture) {
                states.transform *= getTransform();
                sf::Sprite cover{*(loaded_texture->texture)};
                auto alpha = static_cast<std::uint8_t>(
                    m_seconds_to_alpha.clampedTransform(
                        loaded_texture->loaded_since.getElapsedTime().asSeconds()
                    )
                );
                auto grey = should_be_grayed_out ? 2 : 1;
                cover.setColor(sf::Color(255/grey, 255/grey, 255/grey, alpha));
                auto bounds = cover.getGlobalBounds();
                cover.setScale(m_size*0.8f/bounds.width, m_size*0.8f/bounds.height);
                cover.setPosition(m_size*0.1f,m_size*0.1563f);
                target.draw(cover, states);
            }
        }
        sf::CircleShape chart_dif_badge{m_size*0.1f, 30};
        Toolkit::setNormOrigin(chart_dif_badge, 0.5f, 0.5f);
        chart_dif_badge.setPosition(m_size*0.1f, m_size*(0.1563f + 0.15f));
        if (should_be_grayed_out) {
            chart_dif_badge.setFillColor(sf::Color(128,128,128));
        } else {
            if (selected_chart == "BSC") {
                chart_dif_badge.setFillColor(m_resources.BSC_color);
            } else if (selected_chart == "ADV") {
                chart_dif_badge.setFillColor(m_resources.ADV_color);
            } else if (selected_chart == "EXT") {
                chart_dif_badge.setFillColor(m_resources.EXT_color);
            } else {
                chart_dif_badge.setFillColor(
                    Toolkit::HSL(
                        static_cast<int>(std::hash<std::string>{}(selected_chart)),
                        83,
                        49
                    ).TurnToRGB()
                );
            }
        }
        target.draw(chart_dif_badge, states);
        if (not should_be_grayed_out) {
            auto dif = m_song.chart_levels.at(selected_chart);
            sf::Text dif_label{
                std::to_string(dif),
                m_resources.noto_sans_medium,
                static_cast<unsigned int>(m_size*0.15f)
            };
            dif_label.setFillColor(sf::Color::White);
            Toolkit::setNormOrigin(dif_label, 0.5f, 0.5f);
            dif_label.setPosition(m_size*0.1f, m_size*(0.1563f + 0.15f));
            target.draw(dif_label, states);
        }
        sf::Text song_title;
        song_title.setFont(m_resources.noto_sans_medium);
        song_title.setString(m_song.title);
        song_title.setCharacterSize(static_cast<unsigned int>(0.06875f*m_size));
        song_title.setFillColor(sf::Color::White);
        auto song_title_bounds = song_title.getLocalBounds();
        // text is too long : scale it
        if (song_title_bounds.width > 0.88f * m_size) {
            song_title.setScale(0.88f * m_size / song_title_bounds.width, 1.0f);
        }
        song_title.setPosition(18.f/160.f,9.f/160.f);
        target.draw(song_title, states);
    }

    void set_to_global_bounds(sf::RectangleShape& rect, const sf::Text& text) {
        const auto& bounds = text.getGlobalBounds();
        rect.setSize({bounds.width, bounds.height});
        rect.setPosition({bounds.left, bounds.top});
    }

    void ColoredMessagePanel::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        sf::RectangleShape frame{{m_size*0.9f, m_size*0.9f}};
        frame.setFillColor(sf::Color::Black);
        frame.setOutlineThickness(1.f);
        frame.setOutlineColor(m_color);
        frame.setOrigin(frame.getSize().x / 2.f, frame.getSize().y / 2.f);
        frame.setPosition(m_size/2.f, m_size/2.f);
        target.draw(frame, states);

        sf::Text message;
        message.setFont(m_resources.noto_sans_medium);
        message.setString(m_message);
        message.setCharacterSize(static_cast<unsigned int>(0.1f*m_size));
        message.setFillColor(m_color);
        auto bounds = message.getLocalBounds();
        message.setOrigin(bounds.left+bounds.width*0.5f, bounds.top+bounds.height*0.5f);
        auto biggest_side = std::max(bounds.width, bounds.height);
        if (biggest_side > m_size*0.8f) {
            message.setScale(m_size*0.8f / biggest_side, m_size*0.8f / biggest_side);
        }
        message.setPosition(m_size*0.5f, m_size*0.5f);
        target.draw(message, states);
    }
}
