#pragma once

#include <SFML/Graphics.hpp>

#include "SharedResources.hpp"
#include "../../Data/Buttons.hpp"

namespace MusicSelect {
    class OptionPage : public sf::Drawable, public sf::Transformable, public HoldsSharedResources {
    public:
        virtual void click(const Data::Button& button) = 0;
        virtual ~OptionPage() = default;
    };

    class MainOptionPage final : public OptionPage {
    public:
        MainOptionPage();
        void click(const Data::Button& button);
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states);
    }
}
