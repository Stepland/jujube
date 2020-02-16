#pragma once

#include <map>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "../../Data/Buttons.hpp"
#include "../../Toolkit/AffineTransform.hpp"

#include "SharedResources.hpp"

namespace MusicSelect {
    class ButtonHighlight : public sf::Drawable, public sf::Transformable, public HoldsSharedResources {
    public:
        ButtonHighlight(SharedResources& resources);
        void button_pressed(Data::Button button);
    private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        mutable sf::RectangleShape m_highlight;
        mutable std::map<Data::Button, sf::Clock> m_button_presses_history;
        Toolkit::AffineTransform<float> m_time_to_alpha;
    };
}
