#include "ControlPanels.hpp"

#include "../../../Toolkit/NormalizedOrigin.hpp"

namespace MusicSelect {
    void LeftButton::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        sf::CircleShape circle{get_panel_size()*0.4f};
        circle.setFillColor(sf::Color::Black);
        circle.setOutlineThickness(0.02f*get_panel_size());
        circle.setOutlineColor(sf::Color::White);
        Toolkit::set_origin_normalized(circle, 0.5f, 0.5f);
        circle.setPosition(get_panel_size()*0.5f, get_panel_size()*0.5f);
        target.draw(circle, states);

        sf::RectangleShape arrow_part{sf::Vector2f{0.1f,0.4f}*get_panel_size()};
        arrow_part.setFillColor(sf::Color::White);
        Toolkit::set_origin_normalized(arrow_part, 2.f, 0.5f);
        arrow_part.setPosition(get_panel_size()*0.5f, get_panel_size()*0.5f);
        arrow_part.move(0.05f*get_panel_size(), 0.f);
        arrow_part.rotate(45.f);
        target.draw(arrow_part, states);
        arrow_part.rotate(-90.f);
        target.draw(arrow_part, states);
    }

    void RightButton::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        sf::CircleShape circle{get_panel_size()*0.4f};
        circle.setFillColor(sf::Color::Black);
        circle.setOutlineThickness(0.02f*get_panel_size());
        circle.setOutlineColor(sf::Color::White);
        Toolkit::set_origin_normalized(circle, 0.5f, 0.5f);
        circle.setPosition(get_panel_size()*0.5f, get_panel_size()*0.5f);
        target.draw(circle, states);

        sf::RectangleShape arrow_part{sf::Vector2f{0.1f,0.4f}*get_panel_size()};
        arrow_part.setFillColor(sf::Color::White);
        Toolkit::set_origin_normalized(arrow_part, -1.f, 0.5f);
        arrow_part.setPosition(get_panel_size()*0.5f, get_panel_size()*0.5f);
        arrow_part.move(-0.05f*get_panel_size(), 0.f);
        arrow_part.rotate(45.f);
        target.draw(arrow_part, states);
        arrow_part.rotate(-90.f);
        target.draw(arrow_part, states);
    }

    void OptionsButton::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        sf::CircleShape circle{get_panel_size()*0.4f};
        circle.setFillColor(sf::Color::Black);
        circle.setOutlineThickness(0.02f*get_panel_size());
        circle.setOutlineColor(sf::Color::White);
        Toolkit::set_origin_normalized(circle, 0.5f, 0.5f);
        circle.setPosition(get_panel_size()*0.5f, get_panel_size()*0.5f);
        target.draw(circle, states);

        sf::Text label{
            "OPTIONS",
            shared.fallback_font.black,
            static_cast<unsigned int>(0.2f*get_panel_size())
        };
        label.setFillColor(sf::Color::White);
        Toolkit::set_local_origin_normalized(label, 0.5f, 0.5f);
        label.setPosition(get_panel_size()*0.5f, get_panel_size()*0.5f);
        label.setScale(0.8f, 1.0f);
        target.draw(label, states);
    }

    void StartButton::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        sf::CircleShape circle{get_panel_size()*0.4f};
        circle.setFillColor(sf::Color::Black);
        circle.setOutlineThickness(0.02f*get_panel_size());
        circle.setOutlineColor(shared.BSC_color);
        Toolkit::set_origin_normalized(circle, 0.5f, 0.5f);
        circle.setPosition(get_panel_size()*0.5f, get_panel_size()*0.5f);
        target.draw(circle, states);

        sf::Text label{
            "START",
            shared.fallback_font.black,
            static_cast<unsigned int>(0.2f*get_panel_size())
        };
        label.setFillColor(shared.BSC_color);
        Toolkit::set_local_origin_normalized(label, 0.5f, 0.5f);
        label.setPosition(get_panel_size()*0.5f, get_panel_size()*0.5f);
        target.draw(label, states);
    }

    void BackButton::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        sf::CircleShape circle{get_panel_size()*0.4f};
        circle.setFillColor(sf::Color::Black);
        circle.setOutlineThickness(0.02f*get_panel_size());
        circle.setOutlineColor(shared.EXT_color);
        Toolkit::set_origin_normalized(circle, 0.5f, 0.5f);
        circle.setPosition(get_panel_size()*0.5f, get_panel_size()*0.5f);
        target.draw(circle, states);

        sf::Text label{
            "BACK",
            shared.fallback_font.black,
            static_cast<unsigned int>(0.2f*get_panel_size())
        };
        label.setFillColor(shared.EXT_color);
        Toolkit::set_local_origin_normalized(label, 0.5f, 0.5f);
        label.setPosition(get_panel_size()*0.5f, get_panel_size()*0.5f);
        target.draw(label, states);
    }
}