#pragma once

#include <optional>
#include <unordered_map>

#include <SFML/Window.hpp>

#include "Buttons.hpp"

namespace Data {
    class KeyMapping {
    public:
        KeyMapping();
        explicit KeyMapping(std::unordered_map<Button, sf::Keyboard::Key> button_to_key);
        void set_button_to_key(const Button& button, const sf::Keyboard::Key& key);
        std::optional<Button> key_to_button(const sf::Keyboard::Key& key);
        std::optional<sf::Keyboard::Key> button_to_key(const Button& button);
    private:
        std::unordered_map<sf::Keyboard::Key, Button> m_key_to_button;
        std::unordered_map<Button, sf::Keyboard::Key> m_button_to_key;
    };
}
