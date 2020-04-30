#pragma once

#include <map>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "../Input/Buttons.hpp"
#include "../Data/Preferences.hpp"
#include "../Toolkit/AffineTransform.hpp"

namespace Drawables {
    class ButtonHighlight : public sf::Drawable, public sf::Transformable, public Data::HoldsPreferences {
    public:
        ButtonHighlight(Data::Preferences& t_preferences);
        void button_pressed(Input::Button button);
    private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        mutable sf::RectangleShape m_highlight;
        mutable std::map<Input::Button, sf::Clock> m_button_presses_history;
        Toolkit::AffineTransform<float> m_time_to_alpha;
    };
}
