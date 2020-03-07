#pragma once

#include <jbcoe/polymorphic_value.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>

#include "../../Data/Buttons.hpp"
#include "Ribbon.hpp"
#include "SharedResources.hpp"


namespace MusicSelect {

    class PanelLayout;

    class OptionPage : public sf::Drawable, public sf::Transformable, public HoldsSharedResources {
    public:
        OptionPage(SharedResources& resources) : HoldsSharedResources(resources) {update();};
        // Returns true if input was used
        virtual bool handle_raw_input(const sf::Event::KeyEvent& event) = 0;
        virtual ~OptionPage() = default;
        void update();
    };

    class RibbonPage : public OptionPage {
    public:
        RibbonPage(const PanelLayout& layout, SharedResources& resources);
        bool handle_raw_input(const sf::Event::KeyEvent& event) override;
        void button_click(const Data::Button& button);
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
