#pragma once

#include <optional>
#include <unordered_map>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "../../../Input/Events.hpp"
#include "../../../Toolkit/AffineTransform.hpp"
#include "../Resources.hpp"
#include "OptionPage.hpp"

namespace MusicSelect {

    class PressHere final : public sf::Drawable, public sf::Transformable, public HoldsResources {
    public:
        PressHere(ScreenResources& t_resources);
        void update();
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        sf::Text message;
        sf::RectangleShape background;
        sf::Clock time;
        Toolkit::AffineTransform<float> color_anim;
        Toolkit::AffineTransform<float> size_anim;
    };

    class AlreadyMapped final : public sf::Drawable, public sf::Transformable, public HoldsResources {
    public:
        AlreadyMapped(ScreenResources& t_resources);
        void update();
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        sf::RectangleShape background;
    };

    class MappingPreview final : public sf::Drawable, public sf::Transformable, public HoldsResources {
    public:
        MappingPreview(ScreenResources& t_resources, const std::unordered_map<Input::Event, Input::Button>& t_key_to_button);
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        const std::unordered_map<Input::Event, Input::Button>& key_to_button;
        mutable sf::RectangleShape square;
        mutable sf::Text key_label;
    };

    class InputRemap final : public OptionPage {
    public:
        InputRemap(ScreenResources& t_resources);
        bool handle_raw_input(const sf::Event::KeyEvent& event) override;
        const std::string name = "Mapping";
        bool should_exit() override;
        void exit() override;
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        bool cancel_remapping = false;
        std::size_t next_button = 0;
        sf::Clock last_key_press;
        std::optional<sf::Clock> confirm_clock;
        mutable sf::Text confirm_text_top;
        mutable sf::Text confirm_text_bottom;
        mutable sf::Text big_number;
        std::unordered_map<Input::Event, Input::Button> m_key_to_button;
        mutable PressHere press_here_panel;
        mutable AlreadyMapped already_mapped_panel;
        mutable MappingPreview mapping_preview;
    };
}