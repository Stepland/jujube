#pragma once

#include <jbcoe/polymorphic_value.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>

#include "../../../Data/Buttons.hpp"
#include "../Drawables/ControlPanels.hpp"
#include "../Ribbon.hpp"
#include "../SharedResources.hpp"


namespace MusicSelect {

    class PanelLayout;

    class OptionPage : public sf::Drawable, public sf::Transformable, public HoldsSharedResources {
    public:
        OptionPage(SharedResources& t_resources) : HoldsSharedResources(t_resources) {update();};
        // Returns true if input was used
        virtual bool handle_raw_input(const sf::Event::KeyEvent& event) = 0;
        virtual ~OptionPage() = default;
        void update();
        virtual bool should_exit() {return false;};
        virtual void exit() {return;};
        const std::string name;
    };

    class RibbonPage : public OptionPage {
    public:
        RibbonPage(const PanelLayout& layout, SharedResources& t_resources);
        bool handle_raw_input(const sf::Event::KeyEvent& event) override;
        void button_click(const Data::Button& button);
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        Ribbon m_ribbon;
        mutable BackButton back_button;
    };

    class MainOptionPage final : public RibbonPage {
    public:
        MainOptionPage(SharedResources& t_resources);
        const std::string name = "Options";
    private:
        static PanelLayout create_layout(SharedResources& t_resources);
    };

    class InputOptionPage final : public RibbonPage {
    public:
        InputOptionPage(SharedResources& t_resources);
        const std::string name = "Input";
    private:
        static PanelLayout create_layout(SharedResources& t_resources);
    };

    class MarkerSelect final : public RibbonPage {
    public:
        MarkerSelect(SharedResources& t_resources);
        ~MarkerSelect();
        const std::string name = "Marker Select";
    private:
        static PanelLayout create_layout(SharedResources& t_resources);
    };
}
