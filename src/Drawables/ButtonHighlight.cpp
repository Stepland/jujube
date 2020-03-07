#include "ButtonHighlight.hpp"

namespace Drawables {
    ButtonHighlight::ButtonHighlight(Data::Preferences& t_preferences) :
        Data::HoldsPreferences(t_preferences),
        m_highlight(),
        m_time_to_alpha(0.f, 0.25f, 255.f, 0.f)
    {
        m_highlight.setFillColor(sf::Color::Transparent);
        m_highlight.setOutlineThickness(1.f);
    }

    void ButtonHighlight::button_pressed(Data::Button button) {
        m_button_presses_history[button].restart();
    }

    void ButtonHighlight::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        m_highlight.setSize({
            get_panel_size()-(3.f/768.f)*get_screen_width(),
            get_panel_size()-(3.f/768.f)*get_screen_width(),
        });
        m_highlight.setOrigin(m_highlight.getSize().x / 2.f, m_highlight.getSize().y / 2.f);
        auto it = m_button_presses_history.begin();
        while (it != m_button_presses_history.end()) {
            auto elapsed = it->second.getElapsedTime();
            auto coords = Data::button_to_coords(it->first);
            if (elapsed > sf::milliseconds(250)) {
                it = m_button_presses_history.erase(it);
            } else {
                auto alpha = m_time_to_alpha.transform(elapsed.asSeconds());
                m_highlight.setOutlineColor(sf::Color(255,255,0,static_cast<std::size_t>(alpha)));
                m_highlight.setPosition({
                    static_cast<float>(coords.x * get_panel_step()) + get_panel_size()/2.f,
                    static_cast<float>(coords.y * get_panel_step()) + get_panel_size()/2.f
                });
                target.draw(m_highlight, states);
                ++it;
            }
        }
    }
}
