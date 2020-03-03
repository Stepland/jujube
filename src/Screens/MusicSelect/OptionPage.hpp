#pragma once

#include <jbcoe/polymorphic_value.h>
#include <SFML/Graphics.hpp>

#include "../../Data/Buttons.hpp"
#include "Ribbon.hpp"
#include "SharedResources.hpp"


namespace MusicSelect {

    class PanelLayout;

    class OptionPage : public sf::Drawable, public sf::Transformable, public HoldsSharedResources {
    public:
        OptionPage(SharedResources& resources) : HoldsSharedResources(resources) {};
        // An option page should only every recive button presses ranging for 1 to 14
        // Going back a menu should be handled by the MusicSelect screen to avoid destroying the menu
        // while still being in a click() call on it
        virtual void click(const Data::Button& button) = 0;
        virtual ~OptionPage() = default;
        void update();
    };

    class RibbonPage : public OptionPage {
    public:
        RibbonPage(const PanelLayout& layout, SharedResources& resources);
        void click(const Data::Button& button) override;
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        Ribbon m_ribbon;
    };

    class MainOptionPage final : public RibbonPage {
    public:
        MainOptionPage(SharedResources& resources);
    private:
        static PanelLayout create_layout(SharedResources& resources);
    };

    class MarkerSelect final : public RibbonPage {
    public:
        MarkerSelect(SharedResources& resources);
        ~MarkerSelect();
    private:
        static PanelLayout create_layout(SharedResources& resources);
    };
}
