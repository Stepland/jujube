#pragma once

#include <map>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "../../Data/KeyMapping.hpp"
#include "../../Toolkit/AffineTransform.hpp"

#include "SharedResources.hpp"

namespace MusicSelect {
    class ButtonHighlight : public sf::Drawable, public sf::Transformable {
    public:
        ButtonHighlight(SharedResources& resources);
        void button_pressed(Button button);
    private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        SharedResources& m_resources;
        mutable sf::RectangleShape m_highlight;
        mutable std::map<Button, sf::Clock> m_button_presses_history;
        Toolkit::AffineTransform<float> m_time_to_alpha;
    };
}
