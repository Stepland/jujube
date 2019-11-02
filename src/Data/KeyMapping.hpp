#pragma once

#include <optional>
#include <unordered_map>

#include <SFML/Window.hpp>

enum class Button {
    B1,
    B2,
    B3,
    B4,
    B5,
    B6,
    B7,
    B8,
    B9,
    B10,
    B11,
    B12,
    B13,
    B14,
    B15,
    B16,
};

struct ButtonCoords {
    unsigned int x;
    unsigned int y;
};

ButtonCoords toCoord(Button button);

class KeyMapping {
public:
    KeyMapping();
    void setPanelToKey(const Button& button, const sf::Keyboard::Key& key);
    std::optional<Button> key_to_button(const sf::Keyboard::Key& key);
    std::optional<sf::Keyboard::Key> button_to_key(const Button& button);
private:
    std::unordered_map<sf::Keyboard::Key, Button> m_key_to_button;
    std::unordered_map<Button, sf::Keyboard::Key> m_button_to_key;
};
