#include "Events.hpp"

namespace Input {
    KeyToString mappable_button_to_string;

    std::string to_string(const Key& mk) {
        return std::visit(mappable_button_to_string, mk);
    }

    std::regex mappable_button_regex("(.+?)::(.+)");
    std::regex joystick_button_regex("(\\d+)_(\\d+)");

    Key from_string(const std::string& s) {
        std::smatch matches;
        if (not std::regex_match(s, matches, mappable_button_regex)) {
            throw std::runtime_error("Unknown Key : "+s);
        }
        auto device = matches[1].str();
        auto button = matches[2].str();
        if (device == "Keyboard") {
            return string_to_keyboard.at(button);
        } else if (device == "Joystick") {
            std::smatch joystick_matches;
            if (not std::regex_match(button, joystick_matches, joystick_button_regex)) {
                throw std::runtime_error("Invalid Joystick Button : "+button);
            }
            auto joystick_id = std::stoul(matches[1].str());
            auto joystick_button = std::stoul(matches[2].str());
            if (joystick_id >= sf::Joystick::Count) {
                throw std::runtime_error("Unsupported Joystick ID : "+matches[1].str());
            }
            if (joystick_button >= sf::Joystick::ButtonCount) {
                throw std::runtime_error("Unsupported Joystick Button : "+matches[2].str());
            }
            sf::Event::JoystickButtonEvent res;
            res.joystickId = static_cast<unsigned int>(joystick_id);
            res.button = static_cast<unsigned int>(joystick_button);
            return res;
        } else {
            throw std::runtime_error("Unsupported input device : "+device);
        }
    }
}