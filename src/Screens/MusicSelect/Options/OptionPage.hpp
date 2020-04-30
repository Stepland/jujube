#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>

#include "../../../Input/Buttons.hpp"
#include "../Drawables/ControlPanels.hpp"
#include "../Ribbon.hpp"
#include "../Resources.hpp"


namespace MusicSelect {

    class PanelLayout;

    class OptionPage : public sf::Drawable, public sf::Transformable, public HoldsResources {
    public:
        OptionPage(ScreenResources& t_resources) : HoldsResources(t_resources) {update();};
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
        RibbonPage(const PanelLayout& layout, ScreenResources& t_resources);
        bool handle_raw_input(const sf::Event::KeyEvent& event) override;
        void button_click(const Input::Button& button);
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        Ribbon m_ribbon;
        mutable BackButton back_button;
    };

    class MainOptionPage final : public RibbonPage {
    public:
        MainOptionPage(ScreenResources& t_resources);
        const std::string name = "Options";
    private:
        static PanelLayout create_layout(ScreenResources& t_resources);
    };

    class InputOptionPage final : public RibbonPage {
    public:
        InputOptionPage(ScreenResources& t_resources);
        const std::string name = "Input";
    private:
        static PanelLayout create_layout(ScreenResources& t_resources);
    };

    class MarkerSelect final : public RibbonPage {
    public:
        MarkerSelect(ScreenResources& t_resources);
        ~MarkerSelect();
        const std::string name = "Marker Select";
    private:
        static PanelLayout create_layout(ScreenResources& t_resources);
    };
}
