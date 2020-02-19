#pragma once

#include <optional>
#include <string>
#include <unordered_map>

#include <cereal/details/traits.hpp>
#include <cereal/types/string.hpp>
#include <magic_enum.hpp>
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

        friend struct Preferences;
    };
}

namespace cereal {
    // Saving for std::unordered_map<Button, sf::Keyboard::Key> for text based archives
    template <
        class Archive,
        traits::EnableIf<traits::is_text_archive<Archive>::value> = traits::sfinae
    > inline
    void save(Archive& ar, const std::unordered_map<Data::Button, sf::Keyboard::Key>& map ) {
        for(auto&& i : magic_enum::enum_entries<Data::Button>()) {
            auto it = map.find(i.first);
            if (it != map.end()) {
                ar(
                    cereal::make_nvp(
                        std::string{i.second},
                        std::string{magic_enum::enum_name(it->second)}
                    )
                );
            }
        }
    }

    //! Loading for std::unordered_map<Button, sf::Keyboard::Key> for text based archives
    template <
        class Archive,
        traits::EnableIf<traits::is_text_archive<Archive>::value> = traits::sfinae
    > inline
    void load(Archive& ar, std::unordered_map<Data::Button, sf::Keyboard::Key>& map ) {
        map.clear();
        while(true) {
            const auto namePtr = ar.getNodeName();
            if (not namePtr) {
                break;
            }
            std::string button_str = namePtr;
            std::string keyboard_key_str;
            ar(keyboard_key_str);

            auto button = magic_enum::enum_cast<Data::Button>(button_str);
            if (not button.has_value()) {
                throw std::runtime_error("Invalid Button : "+button_str);
            }

            auto keyboard_key = magic_enum::enum_cast<sf::Keyboard::Key>(keyboard_key_str);
            if (not keyboard_key.has_value()) {
                throw std::runtime_error("Invalid Keyboard key : "+keyboard_key_str);
            }

            map.emplace(*button, *keyboard_key);
        }
    }
} // namespace cereal
