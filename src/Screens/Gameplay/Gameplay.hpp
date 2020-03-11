#pragma once

#include <SFML/Graphics.hpp>

#include "../../Data/Chart.hpp"
#include "../../Data/Song.hpp"
#include "../../Data/Score.hpp"
#include "../../Toolkit/Debuggable.hpp"

namespace Gameplay {
    class Screen : public Toolkit::Debuggable {
    public:
        explicit Screen(const Data::SongDifficulty& song_selection);
        Data::Score play_chart(sf::RenderWindow& window);
    private:
        const Data::SongDifficulty& song_selection;
        const Data::Chart chart;
        Data::Score score;
        bool song_finished = false;
    };
};

