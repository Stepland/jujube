#include "Panel.hpp"

#include <SFML/Graphics.hpp>

#include "MusicSelect.hpp"

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
        frame.setOrigin(frame.getSize().x / 2.f, frame.getSize().y / 2.f);
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
        label_text.setOrigin(text_bounds.width / 2.f, text_bounds.height / 2.f);
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
        
    }

    void SongPanel::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        if (m_song.cover) {
            auto loaded_texture = m_resources.covers.async_get(m_song.folder/m_song.cover.value());
            if (loaded_texture) {
                states.transform *= getTransform();
                sf::Sprite cover{*(loaded_texture->texture)};
                auto alpha = static_cast<std::uint8_t>(
                    m_seconds_to_alpha.clampedTransform(
                        loaded_texture->clock.getElapsedTime().asSeconds()
                    )
                );
                cover.setColor(sf::Color(255, 255, 255, alpha));
                auto bounds = cover.getGlobalBounds();
                cover.setScale(m_size / bounds.width, m_size / bounds.height);
                target.draw(cover, states);
            }
        }
    }

    void set_to_global_bounds(sf::RectangleShape& rect, const sf::Text& text) {
        const auto& bounds = text.getGlobalBounds();
        rect.setSize({bounds.width, bounds.height});
        rect.setPosition({bounds.left, bounds.top});
    }
}