#pragma once

#include <SFML/Window.hpp>
#include "../Model/Score.hpp"

namespace Screen {
    class Result {
        const Score& score;
    public:
        explicit Result(const Score& score);
        void display(sf::Window& window);
    };
};
