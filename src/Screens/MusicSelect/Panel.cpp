#include "Panel.hpp"

#include <SFML/Graphics.hpp>

#include "MusicSelect.hpp"

void MusicSelect::ColorPanel::draw(Resources& resources, sf::RenderTarget& target, sf::FloatRect area) {
    sf::RectangleShape panel{{area.width*0.9f, area.height*0.9f}};
    panel.setFillColor(this->color);
    panel.setOrigin(panel.getSize().x / 2.f, panel.getSize().y / 2.f);
    panel.setPosition(area.left+area.width/2.f, area.top+area.height/2.f);
    target.draw(panel);
}

void MusicSelect::CategoryPanel::click(Ribbon& ribbon, std::size_t from_button_index) {
    ribbon.move_to_next_category(from_button_index);
}

void MusicSelect::CategoryPanel::draw(Resources& resources, sf::RenderTarget& target, sf::FloatRect area) {
    sf::RectangleShape red_rectangle;
    red_rectangle.setFillColor(sf::Color::Transparent);
    red_rectangle.setOutlineColor(sf::Color::Red);
    red_rectangle.setOutlineThickness(1.f);

    sf::RectangleShape frame{{area.width*0.9f, area.height*0.9f}};
    frame.setFillColor(sf::Color::Black);
    frame.setOutlineThickness(1.f);
    frame.setOutlineColor(sf::Color::White);
    frame.setOrigin(frame.getSize().x / 2.f, frame.getSize().y / 2.f);
    frame.setPosition(area.left+area.width/2.f, area.top+area.height/2.f);
    target.draw(frame);

    sf::Text top_text;
    top_text.setFont(resources.noto_sans_medium);
    top_text.setString("category");
    top_text.setCharacterSize(50U);
    top_text.setFillColor(sf::Color::White);
    auto bounds = top_text.getLocalBounds();
    top_text.setScale(area.width*0.45f / bounds.width, area.width*0.45f / bounds.width);
    top_text.setPosition(area.left + area.width*0.07f, area.top + area.height*0.07f);
    target.draw(top_text);

    set_to_global_bounds(red_rectangle, top_text);
    target.draw(red_rectangle);

    sf::Text label_text;
    label_text.setFont(resources.noto_sans_medium);
    label_text.setString(this->label);
    label_text.setCharacterSize(100U);
    label_text.setFillColor(sf::Color::White);
    auto text_bounds = label_text.getLocalBounds();
    label_text.setOrigin(text_bounds.width / 2.f, text_bounds.height / 2.f);
    if (text_bounds.height > text_bounds.width) {
        label_text.setScale(area.height*0.8f / text_bounds.height, area.height*0.8f / text_bounds.height);
    } else {
        label_text.setScale(area.width*0.8f / text_bounds.width, area.width*0.8f / text_bounds.width);
    }
    label_text.setPosition(area.left + area.width / 2.f, area.top + area.height / 2.f);
    target.draw(label_text);

    set_to_global_bounds(red_rectangle, label_text);
    target.draw(red_rectangle);
}

void MusicSelect::SongPanel::click(Ribbon& ribbon, std::size_t from_button_index) {
    
}

void MusicSelect::SongPanel::draw(Resources& resources, sf::RenderTarget& target, sf::FloatRect area) {
    sf::Sprite cover;
    cover.setTexture(resources.fallback_cover);
    if (m_song.cover) {
        auto loaded_texture = resources.covers.async_get(m_song.folder/m_song.cover.value());
        if (loaded_texture) {
            cover.setTexture(**loaded_texture, true);
        }
    }
    auto bounds = cover.getGlobalBounds();
    cover.setPosition(area.left, area.top);
    cover.setScale(area.width / bounds.width, area.width / bounds.width);
    target.draw(cover);
}

void MusicSelect::set_to_global_bounds(sf::RectangleShape& rect, const sf::Text& text) {
    const auto& bounds = text.getGlobalBounds();
    rect.setSize({bounds.width, bounds.height});
    rect.setPosition({bounds.left, bounds.top});
}
