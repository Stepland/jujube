#pragma once

#include <optional>
#include <string>
#include <unordered_map>

#include <nlohmann/json.hpp>
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
        explicit KeyMapping(std::unordered_map<Button, Key> button_to_key);
        explicit KeyMapping(std::unordered_map<Key, Button> button_to_key);
        void set_button_to_key(const Button& button, const Key& key);
        std::optional<Button> key_to_button(const Key& key);
        std::optional<Key> button_to_key(const Button& button);
    private:
        std::unordered_map<Key, Button> m_key_to_button;
        std::unordered_map<Button, Key> m_button_to_key;

        friend struct Data::Preferences;
        friend void to_json(nlohmann::json& j, const KeyMapping& km);
        friend void from_json(const nlohmann::json& j, KeyMapping& km);
    };
}
