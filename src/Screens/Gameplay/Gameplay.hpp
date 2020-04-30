#pragma once

#include <atomic>

#include <SFML/Graphics.hpp>

#include "../../Data/Chart.hpp"
#include "../../Data/Note.hpp"
#include "../../Data/Song.hpp"
#include "../../Data/Score.hpp"
#include "../../Input/Buttons.hpp"
#include "../../Input/Events.hpp"
#include "../../Toolkit/Debuggable.hpp"
#include "PreciseMusic.hpp"
#include "Silence.hpp"
#include "GradedNote.hpp"

namespace Gameplay {
    class Screen : public Toolkit::Debuggable {
    public:
        explicit Screen(const Data::SongDifficulty& song_selection);
        Data::Score play_chart(sf::RenderWindow& window);
    private:
        void render(sf::RenderWindow& window);

        std::function<sf::Time(void)> getPlayingOffset;
        std::function<sf::SoundSource::Status(void)> getStatus;

        void handle_raw_event(const Input::Event& event, const sf::Time& playing_offset);
        void handle_button(const Input::Button& button, const sf::Time& playing_offset);

        const Data::SongDifficulty& song_selection;
        const Data::Chart chart;

        std::set<GradedNote> notes_passed;
        std::set<Data::Note> notes_left;

        std::atomic<bool> song_finished = false;
    };

    struct GetPlayingOffsetVisitor {
        std::function<sf::Time(void)> operator() (const PreciseMusic& pm) {
            return [&pm](){return pm.getPrecisePlayingOffset();};
        }
        std::function<sf::Time(void)> operator() (const Silence& s) {
            return [&s](){return s.getPlayingOffset();};
        }
    };

    struct GetStatusVisitor {
        std::function<sf::SoundSource::Status(void)> operator() (const PreciseMusic& pm) {
            return [&pm](){return pm.getStatus();};
        }
        std::function<sf::SoundSource::Status(void)> operator() (const Silence& s) {
            return [&s](){return s.getStatus();};
        }
    };
}

