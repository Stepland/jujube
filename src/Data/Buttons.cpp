#include "Buttons.hpp"

namespace Data {
    ButtonCoords button_to_coords(Button button) {
        auto num = static_cast<std::size_t>(button);
        return {num % 4, num / 4};
    }

    std::size_t button_to_index(Button button) {
        return static_cast<std::size_t>(button);
    }

    std::optional<Button> coords_to_button(ButtonCoords button_coords) {
        if (
            button_coords.x < 4 and
            button_coords.y < 4
        ) {
            return static_cast<Button>(button_coords.x + 4*button_coords.y);
        } else {
            return {};
        }
    }

    std::optional<Button> index_to_button(std::size_t index) {
        if (index < 16) {
            return static_cast<Button>(index);
        } else {
            return {};
        }
    }
}