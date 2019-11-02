#include "KeyMaping.hpp"

PanelCoords toCoord(PanelEnum panel) {
    auto num = static_cast<int>(panel);
    return {num % 4, num / 4};
}

KeyMaping::KeyMaping() {
    m_key_to_panel[sf::Keyboard::Num1] = PanelEnum::P1;
    m_key_to_panel[sf::Keyboard::Num2] = PanelEnum::P2;
    m_key_to_panel[sf::Keyboard::Num3] = PanelEnum::P3;
    m_key_to_panel[sf::Keyboard::Num4] = PanelEnum::P4;
    m_key_to_panel[sf::Keyboard::A] = PanelEnum::P5;
    m_key_to_panel[sf::Keyboard::Z] = PanelEnum::P6;
    m_key_to_panel[sf::Keyboard::E] = PanelEnum::P7;
    m_key_to_panel[sf::Keyboard::R] = PanelEnum::P8;
    m_key_to_panel[sf::Keyboard::Q] = PanelEnum::P9;
    m_key_to_panel[sf::Keyboard::S] = PanelEnum::P10;
    m_key_to_panel[sf::Keyboard::D] = PanelEnum::P11;
    m_key_to_panel[sf::Keyboard::F] = PanelEnum::P12;
    m_key_to_panel[sf::Keyboard::W] = PanelEnum::P13;
    m_key_to_panel[sf::Keyboard::X] = PanelEnum::P14;
    m_key_to_panel[sf::Keyboard::C] = PanelEnum::P15;
    m_key_to_panel[sf::Keyboard::V] = PanelEnum::P16;

    for (const auto& [key, panel] : m_key_to_panel) {
        m_panel_to_key[panel] = key;
    }
}

void KeyMaping::setPanelToKey(const PanelEnum& panel, const sf::Keyboard::Key& key) {
    if (m_key_to_panel.find(key) != m_key_to_panel.end()) {
        m_panel_to_key.erase(m_key_to_panel[key]);
        m_key_to_panel.erase(key);
    }
    m_panel_to_key[panel] = key;
    m_key_to_panel[key] = panel;
}

std::optional<PanelEnum> KeyMaping::key_to_panel(const sf::Keyboard::Key& key) {
    try {
        return m_key_to_panel.at(key);
    } catch(const std::exception& e) {
        return {};
    }
}

std::optional<sf::Keyboard::Key> KeyMaping::panel_to_key(const PanelEnum& panel) {
    try {
        return m_panel_to_key.at(panel);
    } catch(const std::exception& e) {
        return {};
    }
}
