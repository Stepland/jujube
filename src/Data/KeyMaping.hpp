#pragma once

#include <optional>
#include <unordered_map>

#include <SFML/Window.hpp>

enum class PanelEnum {
    P1,
    P2,
    P3,
    P4,
    P5,
    P6,
    P7,
    P8,
    P9,
    P10,
    P11,
    P12,
    P13,
    P14,
    P15,
    P16,
};

struct PanelCoords {
    unsigned int x;
    unsigned int y;
};

PanelCoords toCoord(PanelEnum panel);

class KeyMaping {
public:
    KeyMaping();
    void setPanelToKey(const PanelEnum& panel, const sf::Keyboard::Key& key);
    std::optional<PanelEnum> key_to_panel(const sf::Keyboard::Key& key);
    std::optional<sf::Keyboard::Key> panel_to_key(const PanelEnum& panel);
private:
    std::unordered_map<sf::Keyboard::Key, PanelEnum> m_key_to_panel;
    std::unordered_map<PanelEnum, sf::Keyboard::Key> m_panel_to_key;
};
