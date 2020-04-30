#pragma once

#include <optional>
#include <string>
#include <unordered_map>

#include <nlohmann/json.hpp>
#include <magic_enum.hpp>
#include <SFML/Window.hpp>

#include "Buttons.hpp"
#include "Events.hpp"

namespace Data {
    struct Preferences;
}

namespace Input {
    class KeyMapping {
    public:
        KeyMapping();
        explicit KeyMapping(std::unordered_map<Button, Event> button_to_key);
        explicit KeyMapping(std::unordered_map<Event, Button> button_to_key);
        void set_button_to_key(const Button& button, const Event& key);
        std::optional<Button> key_to_button(const Event& key);
        std::optional<Event> button_to_key(const Button& button);
    private:
        std::unordered_map<Event, Button> m_key_to_button;
        std::unordered_map<Button, Event> m_button_to_key;

        friend struct Data::Preferences;
        friend void to_json(nlohmann::json& j, const KeyMapping& km);
        friend void from_json(const nlohmann::json& j, KeyMapping& km);
    };
}
