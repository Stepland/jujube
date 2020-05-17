#pragma once

#include <string>

#include "../Panels/Panel.hpp"
#include "../Resources.hpp"
#include "OptionPage.hpp"

namespace MusicSelect {
    class AudioOffset final : public OptionPage {
    public:
        AudioOffset(ScreenResources& t_resources);
        bool handle_raw_input(const sf::Event::KeyEvent& event) override;
        void handle_button(const Input::Button& button);
        const std::string name = "Audio Offset";
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        mutable BackButton back_button;
        mutable ColoredMessagePanel minus;
        mutable ColoredMessagePanel plus;
        mutable ColoredMessagePanel display;
    };

    std::string audio_offset_to_string(const sf::Time& audio_offset);
}