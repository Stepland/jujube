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
        virtual void click(const Data::Button& button) = 0;
        virtual ~OptionPage() = default;
    private:
    
    };

    class MainOptionPage final : virtual public OptionPage, public Ribbon {
    public:
        MainOptionPage(SharedResources& resources);
        void click(const Data::Button& button) override;
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        static PanelLayout create_layout(SharedResources& resources);
    };

    class MarkerSelect final : virtual public OptionPage, public Ribbon {
    public:
        MarkerSelect(SharedResources& resources);
        void click(const Data::Button& button) override;
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        static PanelLayout create_layout(SharedResources& resources);
    };
}
