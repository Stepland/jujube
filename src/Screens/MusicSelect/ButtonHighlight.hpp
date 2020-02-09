#pragma once

#include <map>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "../../Data/KeyMapping.hpp"
#include "../../Toolkit/AffineTransform.hpp"

namespace MusicSelect {
    class ButtonHighlight : public sf::Drawable, public sf::Transformable {
    public:
        ButtonHighlight(const float& panel_size, const float& panel_spacing);
        void button_pressed(Button button);
    private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        const float& m_panel_size;
        const float& m_panel_spacing;
        mutable sf::RectangleShape m_highlight;
        mutable std::map<Button, sf::Clock> m_button_presses_history;
        Toolkit::AffineTransform<float> m_time_to_alpha;
    };
}
