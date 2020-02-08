#include "KeyMapping.hpp"

ButtonCoords toCoords(Button button) {
    auto num = static_cast<unsigned int>(button);
    return {num % 4, num / 4};
}

unsigned int toIndex(Button button) {
    return static_cast<unsigned int>(button);
}

std::optional<Button> fromCoords(ButtonCoords button_coords) {
    if (
        button_coords.x < 4 and
        button_coords.y < 4
    ) {
        return static_cast<Button>(button_coords.x + 4*button_coords.y);
    } else {
        return {};
    }
}

std::optional<Button> fromIndex(unsigned int index) {
    if (index < 16) {
        return static_cast<Button>(index);
    } else {
        return {};
    }
}

KeyMapping::KeyMapping() {
    m_key_to_button[sf::Keyboard::Num1] = Button::B1;
    m_key_to_button[sf::Keyboard::Num2] = Button::B2;
    m_key_to_button[sf::Keyboard::Num3] = Button::B3;
    m_key_to_button[sf::Keyboard::Num4] = Button::B4;
    m_key_to_button[sf::Keyboard::A] = Button::B5;
    m_key_to_button[sf::Keyboard::Z] = Button::B6;
    m_key_to_button[sf::Keyboard::E] = Button::B7;
    m_key_to_button[sf::Keyboard::R] = Button::B8;
    m_key_to_button[sf::Keyboard::Q] = Button::B9;
    m_key_to_button[sf::Keyboard::S] = Button::B10;
    m_key_to_button[sf::Keyboard::D] = Button::B11;
    m_key_to_button[sf::Keyboard::F] = Button::B12;
    m_key_to_button[sf::Keyboard::W] = Button::B13;
    m_key_to_button[sf::Keyboard::X] = Button::B14;
    m_key_to_button[sf::Keyboard::C] = Button::B15;
    m_key_to_button[sf::Keyboard::V] = Button::B16;

    for (const auto& [key, panel] : m_key_to_button) {
        m_button_to_key[panel] = key;
    }
}

void KeyMapping::set_button_to_key(const Button& button, const sf::Keyboard::Key& key) {
    if (m_key_to_button.find(key) != m_key_to_button.end()) {
        m_button_to_key.erase(m_key_to_button[key]);
        m_key_to_button.erase(key);
    }
    m_button_to_key[button] = key;
    m_key_to_button[key] = button;
}

std::optional<Button> KeyMapping::key_to_button(const sf::Keyboard::Key& key) {
    if (m_key_to_button.find(key) == m_key_to_button.end()) {
        return {};
    } else {
        return m_key_to_button.at(key);
    }
}

std::optional<sf::Keyboard::Key> KeyMapping::button_to_key(const Button& button) {
    if (m_button_to_key.find(button) == m_button_to_key.end()) {
        return {};
    } else {
        return m_button_to_key.at(button);
    }
}
