#pragma once

#include <SFML/Window.hpp>
#include "../Data/Chart.hpp"
#include "../Data/Score.hpp"

namespace Screen {
    class Gameplay {
        const Data::Chart& chart;
    public:
        explicit Gameplay(const Data::Chart& selected_chart);
        Score play_chart(sf::Window& window) const;
    };
};

