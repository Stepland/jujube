#pragma once

#include <atomic>
#include <deque>

#include <SFML/Graphics.hpp>

#include "../../Data/Chart.hpp"
#include "../../Data/Note.hpp"
#include "../../Data/Song.hpp"
#include "../../Data/Score.hpp"
#include "../../Resources/Marker.hpp"
#include "../../Input/Buttons.hpp"
#include "../../Input/Events.hpp"
#include "../../Toolkit/Debuggable.hpp"
#include "PreciseMusic.hpp"
#include "Silence.hpp"
#include "GradedNote.hpp"
#include "Resources.hpp"

namespace Gameplay {
    class Screen : Toolkit::Debuggable, HoldsResources {
    public:
        explicit Screen(const Data::SongDifficulty& song_selection, ScreenResources& t_resources);
        void play_chart(sf::RenderWindow& window);
    private:
        void render(sf::RenderWindow& window);

        std::function<sf::Time(void)> getPlayingOffset;
        std::function<sf::SoundSource::Status(void)> getStatus;

        void handle_raw_event(const Input::Event& event, const sf::Time& music_time);
        void handle_mouse_click(const sf::Event::MouseButtonEvent& mouse_button_event, const sf::Time& music_time);
        void handle_button(const Input::Button& button, const sf::Time& music_time);

        const Data::SongDifficulty& song_selection;
        const Data::Chart chart;
        const Resources::Marker& marker;

        std::deque<std::atomic<GradedNote>> notes;
        std::atomic<std::size_t> note_index;
        // moves note_cursor forward to the first note that has to be conscidered for judging
        // marks every note between the old and the new position as missed
        void update_note_index(const sf::Time& music_time);

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

