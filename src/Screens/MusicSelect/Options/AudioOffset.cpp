#include "AudioOffset.hpp"

namespace MusicSelect {
    AudioOffset::AudioOffset(ScreenResources& t_resources) :
        OptionPage(t_resources),
        back_button(t_resources),
        minus(t_resources, sf::Color::White, "-"),
        plus(t_resources, sf::Color::White, "+"),
        display(
            t_resources,
            sf::Color::White,
            audio_offset_to_string(t_resources.preferences.options.audio_offset)
        )
    {
    }

    bool AudioOffset::handle_raw_input(const sf::Event::KeyEvent& event) {
        auto button = preferences.key_mapping.key_to_button(event.code);
        if (not button) {
            return false;
        }
        auto button_index = Input::button_to_index(*button);
        if (button_index > 13) {
            return false;
        }
        handle_button(*button);
        return true;
    }

    void AudioOffset::handle_button(const Input::Button& button) {
        shared.button_highlight.button_pressed(button);
        switch (button) {
        case Input::Button::B1:
            preferences.options.audio_offset -= sf::milliseconds(1);
            display.m_message = audio_offset_to_string(preferences.options.audio_offset);
            break;
        case Input::Button::B3:
            preferences.options.audio_offset += sf::milliseconds(1);
            display.m_message = audio_offset_to_string(preferences.options.audio_offset);
            break;
        default:
            break;
        }
    }

    void AudioOffset::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        minus.setPosition(0.f, 0.f);
        target.draw(minus, states);
        display.setPosition(get_panel_step(), 0.f);
        target.draw(display, states);
        plus.setPosition(2.f*get_panel_step(), 0.f);
        target.draw(plus, states);
        back_button.setPosition(2.f*get_panel_step(), 3.f*get_panel_step());
        target.draw(back_button, states);
    }

    std::string audio_offset_to_string(const sf::Time& audio_offset) {
        return std::to_string(audio_offset.asMilliseconds())+"ms";
    }
}
