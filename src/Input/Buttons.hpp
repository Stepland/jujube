#pragma once

#include <cstddef>
#include <optional>

namespace Input {
    enum class Button : std::size_t {
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
        std::size_t x;
        std::size_t y;
    };

    ButtonCoords button_to_coords(Button button);
    std::size_t button_to_index(Button button);
    std::optional<Button> coords_to_button(ButtonCoords button_coords);
    std::optional<Button> index_to_button(std::size_t index);
}