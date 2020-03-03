#include "SubpagePanel.hpp"

#include "../OptionPage.hpp"

namespace MusicSelect {
    void SubpagePanel::click(Ribbon&, const Data::Button&) {
        m_resources.options_state.push(*m_subpage);
    }

    void SubpagePanel::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        sf::RectangleShape frame{{get_size()*0.9f, get_size()*0.9f}};
        frame.setFillColor(sf::Color::Black);
        frame.setOutlineThickness(1.f);
        frame.setOutlineColor(sf::Color::White);
        frame.setOrigin(frame.getSize().x / 2.f, frame.getSize().y / 2.f);
        frame.setPosition(get_size()/2.f, get_size()/2.f);
        target.draw(frame, states);

        sf::Text message;
        message.setFont(m_resources.fallback_font.medium);
        message.setString(sf::String::fromUtf8(m_name.begin(), m_name.end()));
        message.setCharacterSize(static_cast<unsigned int>(0.1f*get_size()));
        message.setFillColor(sf::Color::White);
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