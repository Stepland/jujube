#include "Panel.hpp"

#include <SFML/Graphics.hpp>

#include "MusicSelect.hpp"

void MusicSelect::CategoryPanel::click(Screen& screen) {
    
}

void MusicSelect::CategoryPanel::draw(Resources& resources, sf::RenderTarget& target, sf::FloatRect area) {
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
    top_text.setCharacterSize(12U);
    top_text.setFillColor(sf::Color::White);
    auto bounds = top_text.getLocalBounds();
    top_text.setScale(area.width*0.45f / bounds.width, area.width*0.45f / bounds.width);
    top_text.setPosition(area.left + area.width*0.15f, area.top + area.height*0.15f);

    sf::Text label_text;
    label_text.setFont(resources.noto_sans_medium);
    label_text.setString(this->label);
    label_text.setCharacterSize(24U);
    label_text.setFillColor(sf::Color::White);
    auto text_bounds = label_text.getLocalBounds();
    label_text.setOrigin(text_bounds.width / 2.f, text_bounds.height / 2.f);
    if (text_bounds.height > text_bounds.width) {
        label_text.setScale(area.height*0.8f / text_bounds.height, area.height*0.8f / text_bounds.height);
    } else {
        label_text.setScale(area.width*0.8f / text_bounds.width, area.width*0.8f / text_bounds.width);
    }
    label_text.setPosition(area.left + area.width / 2.f, area.top + area.height / 2.f);
}



void MusicSelect::SongPanel::click(Screen& screen) {
    
}

void MusicSelect::SongPanel::draw(Resources& resources, sf::RenderTarget& target, sf::FloatRect area) {
    
}
