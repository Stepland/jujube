#pragma once

#include <map>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "../../Data/KeyMapping.hpp"
#include "../../Toolkit/AffineTransform.hpp"

namespace MusicSelect {
    class ButtonHighlight : public sf::Drawable {
    public:
        explicit ButtonHighlight(std::size_t t_panel_size);
        void button_pressed(Button button);
    private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        std::size_t panel_size;
        mutable sf::RectangleShape highlight;
        mutable std::map<Button, sf::Clock> button_presses_history;
        Toolkit::AffineTransform<float> time_to_alpha;
    };
}
