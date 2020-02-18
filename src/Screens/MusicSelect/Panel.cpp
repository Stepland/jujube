#include "Panel.hpp"

#include <functional>

#include <SFML/Graphics.hpp>

#include "../../Toolkit/HSL.hpp"
#include "../../Toolkit/NormalizedOrigin.hpp"

#include "MusicSelect.hpp"
#include "SharedResources.hpp"

namespace MusicSelect {
    Panel::Panel(SharedResources& resources) : HoldsSharedResources(resources) {
        
    }

    float Panel::get_size() const {
        return get_panel_size();
    }
    
    void ColorPanel::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        sf::RectangleShape panel{{get_size()*0.9f, get_size()*0.9f}};
        panel.setFillColor(m_color);
        states.transform *= getTransform();
        target.draw(panel, states);
    }

    void CategoryPanel::click(Ribbon& ribbon, std::size_t from_button_index) {
        ribbon.move_to_next_category(from_button_index);
    }

    void CategoryPanel::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();

        sf::RectangleShape frame{{get_size()*0.9f, get_size()*0.9f}};
        frame.setFillColor(sf::Color::Black);
        frame.setOutlineThickness(1.f);
        frame.setOutlineColor(sf::Color::White);
        Toolkit::set_origin_normalized(frame, 0.5f, 0.5f);
        frame.setPosition(get_size()*0.5f, get_size()*0.5f);
        target.draw(frame, states);

        sf::Text category_label{
            "category",
            m_resources.noto_sans_medium,
            static_cast<unsigned int>(get_size()*0.1f)
        };
        category_label.setFillColor(sf::Color::White);
        auto bounds = category_label.getLocalBounds();
        category_label.setPosition(get_size()*0.1f, get_size()*0.1f);
        target.draw(category_label, states);

        sf::Text label_text{
            m_label,
            m_resources.noto_sans_medium,
            static_cast<unsigned int>(get_size()*0.7f)
        };
        label_text.setFillColor(sf::Color::White);
        Toolkit::set_origin_normalized(label_text, 0.5f, 0.5f);
        auto text_bounds = label_text.getGlobalBounds();
        if (text_bounds.width > get_size()*0.6f) {
            label_text.setScale(get_size()*0.6f / text_bounds.width, get_size()*0.6f / text_bounds.width);
        }
        label_text.setPosition(get_size()*0.5f, get_size()*0.55f);
        target.draw(label_text, states);
    }

    void SongPanel::click(Ribbon& ribbon, std::size_t from_button_index) {
        if (selected_chart.has_value()) {
            // The song was already selected : look for the next chart in order
            auto it = m_song->chart_levels.upper_bound(*selected_chart);
            if (it != m_song->chart_levels.cend()) {
                selected_chart = it->first;
            } else {
                selected_chart = m_song->chart_levels.cbegin()->first;
            }
            m_resources.selected_panel->last_click.restart();
            m_resources.selected_panel->is_first_click = false;
        } else {
            // Look for the first chart with dif greater or equal to the last select one
            // or else select the first chart
            auto it = m_song->chart_levels.lower_bound(m_resources.get_last_selected_difficulty());
            if (it != m_song->chart_levels.cend()) {
                selected_chart = it->first;
            } else {
                selected_chart = m_song->chart_levels.cbegin()->first;
            }
            // The song was not selected before : first unselect the last one
            if (m_resources.selected_panel.has_value()) {
                m_resources.selected_panel->panel.unselect();
            }
            m_resources.selected_panel.emplace(TimedSelectedPanel{*this});
        }
    }

    void SongPanel::unselect() {
        selected_chart.reset();
    }

    std::optional<ChartSelection> SongPanel::get_selected_chart() const {
        if (selected_chart) {
            return ChartSelection{*m_song, *selected_chart};
        } else {
            return {};
        }
    }

    void SongPanel::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        auto last_selected_chart = m_resources.get_last_selected_difficulty();
        // We should gray out the panel if the currently selected difficulty doesn't exist for this song
        bool should_be_grayed_out = m_song->chart_levels.find(last_selected_chart) == m_song->chart_levels.end();
        if (m_song->cover) {
            auto loaded_texture = m_resources.covers.async_get(m_song->folder/m_song->cover.value());
            if (loaded_texture) {
                sf::Sprite cover{*(loaded_texture->texture)};
                auto alpha = static_cast<std::uint8_t>(
                    m_seconds_to_alpha.clampedTransform(
                        loaded_texture->loaded_since.getElapsedTime().asSeconds()
                    )
                );
                auto grey = should_be_grayed_out ? 2 : 1;
                cover.setColor(sf::Color(255/grey, 255/grey, 255/grey, alpha));
                auto bounds = cover.getGlobalBounds();
                cover.setScale(get_size()*0.8f/bounds.width, get_size()*0.8f/bounds.height);
                cover.setPosition(get_size()*0.1f,get_size()*0.1563f);
                target.draw(cover, states);
            }
        }
        sf::CircleShape chart_dif_badge{get_size()*0.1f, 30};
        Toolkit::set_origin_normalized(chart_dif_badge, 0.5f, 0.5f);
        chart_dif_badge.setPosition(get_size()*0.1f, get_size()*(0.1563f + 0.15f));
        if (should_be_grayed_out) {
            chart_dif_badge.setFillColor(sf::Color(128,128,128));
        } else {
            chart_dif_badge.setFillColor(
                m_resources.get_chart_color(last_selected_chart)
            );
        }
        target.draw(chart_dif_badge, states);
        if (not should_be_grayed_out) {
            auto dif = m_song->chart_levels.at(last_selected_chart);
            sf::Text dif_label{
                std::to_string(dif),
                m_resources.noto_sans_medium,
                static_cast<unsigned int>(get_size()*0.15f)
            };
            dif_label.setFillColor(sf::Color::White);
            Toolkit::set_origin_normalized(dif_label, 0.5f, 0.5f);
            dif_label.setPosition(get_size()*0.1f, get_size()*(0.1563f + 0.15f));
            target.draw(dif_label, states);
        }
        sf::Text song_title;
        song_title.setFont(m_resources.noto_sans_medium);
        song_title.setString(m_song->title);
        song_title.setCharacterSize(static_cast<unsigned int>(0.06875f*get_size()));
        song_title.setFillColor(sf::Color::White);
        auto song_title_bounds = song_title.getLocalBounds();
        // text is too long : scale it
        if (song_title_bounds.width > 0.88f * get_size()) {
            song_title.setScale(0.88f * get_size() / song_title_bounds.width, 1.0f);
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
        sf::RectangleShape frame{{get_size()*0.9f, get_size()*0.9f}};
        frame.setFillColor(sf::Color::Black);
        frame.setOutlineThickness(1.f);
        frame.setOutlineColor(m_color);
        frame.setOrigin(frame.getSize().x / 2.f, frame.getSize().y / 2.f);
        frame.setPosition(get_size()/2.f, get_size()/2.f);
        target.draw(frame, states);

        sf::Text message;
        message.setFont(m_resources.noto_sans_medium);
        message.setString(m_message);
        message.setCharacterSize(static_cast<unsigned int>(0.1f*get_size()));
        message.setFillColor(m_color);
        auto bounds = message.getLocalBounds();
        message.setOrigin(bounds.left+bounds.width*0.5f, bounds.top+bounds.height*0.5f);
        auto biggest_side = std::max(bounds.width, bounds.height);
        if (biggest_side > get_size()*0.8f) {
            message.setScale(get_size()*0.8f / biggest_side, get_size()*0.8f / biggest_side);
        }
        message.setPosition(get_size()*0.5f, get_size()*0.5f);
        target.draw(message, states);
    }
}
