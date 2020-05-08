#pragma once

#include <experimental/set> // erase_if
#include <experimental/map> // erase_if

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "../Input/Events.hpp"
#include "../Data/Preferences.hpp"
#include "../Toolkit/AffineTransform.hpp"

namespace Drawables {
    class ButtonHighlight : public sf::Drawable, public sf::Transformable, public Data::HoldsPreferences {
    public:
        ButtonHighlight(Data::Preferences& t_preferences);
        void button_pressed(Input::Button button);
        void handle_button_event(Input::ButtonEvent button_event);
        void update();
    private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        mutable sf::RectangleShape m_highlight;
        std::set<Input::Button> m_pressed_buttons;
        std::map<Input::Button, sf::Clock> m_released_buttons;
        Toolkit::AffineTransform<float> m_time_to_alpha;
    };
}
