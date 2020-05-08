#include "ButtonHighlight.hpp"

#include <algorithm>

namespace Drawables {
    ButtonHighlight::ButtonHighlight(Data::Preferences& t_preferences) :
        Data::HoldsPreferences(t_preferences),
        m_highlight(),
        m_time_to_alpha(0.f, 0.25f, 255.f, 0.f)
    {
        m_highlight.setFillColor(sf::Color::Transparent);
        m_highlight.setOutlineThickness(1.f);
    }

    void ButtonHighlight::button_pressed(Input::Button button) {
        m_pressed_buttons.erase(button);
        m_released_buttons[button].restart();
    }


    void ButtonHighlight::handle_button_event(Input::ButtonEvent button_event) {
        switch (button_event.type) {
        case Input::EventType::Pressed:
            m_pressed_buttons.insert(button_event.button);
            m_released_buttons.erase(button_event.button);
            break;
        case Input::EventType::Released:
            m_pressed_buttons.erase(button_event.button);
            m_released_buttons[button_event.button].restart();
            break;
        }
    }

    // TODO: use map::erase_if when moving to C++20
    void ButtonHighlight::update() {
        for(auto it = m_released_buttons.begin();it != m_released_buttons.end();) {
            if (it->second.getElapsedTime() > sf::milliseconds(250)) {
                it = m_released_buttons.erase(it);
            } else {
                ++it;
            }
        }
    }

    void ButtonHighlight::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        m_highlight.setSize({
            get_panel_size()-(3.f/768.f)*get_screen_width(),
            get_panel_size()-(3.f/768.f)*get_screen_width(),
        });
        m_highlight.setOrigin(m_highlight.getSize().x / 2.f, m_highlight.getSize().y / 2.f);
        m_highlight.setOutlineColor(sf::Color::Yellow);
        for (auto&& button : m_pressed_buttons) {
            auto coords = Input::button_to_coords(button);
            m_highlight.setPosition({
                static_cast<float>(coords.x * get_panel_step()) + get_panel_size()/2.f,
                static_cast<float>(coords.y * get_panel_step()) + get_panel_size()/2.f
            });
            target.draw(m_highlight, states);
        }
        for (auto&& [button, timer] : m_released_buttons) {
            auto coords = Input::button_to_coords(button);
            auto alpha = m_time_to_alpha.transform(timer.getElapsedTime().asSeconds());
            m_highlight.setOutlineColor(sf::Color(255,255,0,static_cast<std::size_t>(alpha)));
            m_highlight.setPosition({
                static_cast<float>(coords.x * get_panel_step()) + get_panel_size()/2.f,
                static_cast<float>(coords.y * get_panel_step()) + get_panel_size()/2.f
            });
            target.draw(m_highlight, states);
        }
    }
}