#include "KeyMapping.hpp"

namespace Data {
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

        for (const auto& [key, button] : m_key_to_button) {
            m_button_to_key[button] = key;
        }
    }

    KeyMapping::KeyMapping(std::unordered_map<Button, MappableKey> button_to_key) : m_button_to_key(button_to_key) {
        for (auto &&[button, key] : m_button_to_key) {
            m_key_to_button[key] = button;
        }
        assert((m_button_to_key.size() == m_key_to_button.size()));
    }

    KeyMapping::KeyMapping(std::unordered_map<MappableKey, Button> key_to_button) : m_key_to_button(key_to_button) {
        for (auto &&[key, button] : m_key_to_button) {
            m_button_to_key[button] = key;
        }
        assert((m_button_to_key.size() == m_key_to_button.size()));
    }

    void KeyMapping::set_button_to_key(const Button& button, const MappableKey& key) {
        if (m_key_to_button.find(key) != m_key_to_button.end()) {
            m_button_to_key.erase(m_key_to_button[key]);
            m_key_to_button.erase(key);
        }
        m_button_to_key[button] = key;
        m_key_to_button[key] = button;
    }

    std::optional<Button> KeyMapping::key_to_button(const MappableKey& key) {
        if (m_key_to_button.find(key) == m_key_to_button.end()) {
            return {};
        } else {
            return m_key_to_button.at(key);
        }
    }

    std::optional<MappableKey> KeyMapping::button_to_key(const Button& button) {
        if (m_button_to_key.find(button) == m_button_to_key.end()) {
            return {};
        } else {
            return m_button_to_key.at(button);
        }
    }

    void to_json(nlohmann::json& j, const KeyMapping& km) {
        for(auto const&[button, key] : km.m_button_to_key) {
            j["B"+(std::to_string(button_to_index(button)+1))] = to_string(key);
        }
    }

    void from_json(const nlohmann::json& j, KeyMapping& km) {
        std::unordered_map<Button, MappableKey> map;
        for (std::size_t i = 0; i < 16; i++) {
            map[*index_to_button(i)] = from_string(j.at("B"+std::to_string(i+1)));
        }
        km = KeyMapping{map};
    }
}

