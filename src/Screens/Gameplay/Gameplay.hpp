#pragma once

#include <atomic>
#include <deque>
#include <functional>
#include <memory>
#include <unordered_map>

#include <SFML/Graphics.hpp>

#include "../../Data/Chart.hpp"
#include "../../Data/GradedNote.hpp"
#include "../../Data/Note.hpp"
#include "../../Data/Song.hpp"
#include "../../Data/Score.hpp"
#include "../../Drawables/GradedDensityGraph.hpp"
#include "../../Resources/Marker.hpp"
#include "../../Input/Buttons.hpp"
#include "../../Input/Events.hpp"
#include "../../Toolkit/AffineTransform.hpp"
#include "../../Toolkit/Debuggable.hpp"
#include "AbstractMusic.hpp"
#include "Resources.hpp"
#include "TimedEventsQueue.hpp"

namespace Gameplay {
    class Screen : public Toolkit::Debuggable, public HoldsResources {
    public:
        explicit Screen(const Data::SongDifficulty& song_selection, ScreenResources& t_resources);
        void play_chart(sf::RenderWindow& window);
    private:
        void draw_debug() override;
        
        void render(sf::RenderWindow& window);
        // Draw a normal note on marker_layer
        void draw_tap_note(const Data::GradedNote& note, const sf::Time& music_time);
        // Draw the long note tail on ln_tail_layer and its markers on marker_layer
        void draw_long_note(const Data::GradedNote& note, const sf::Time& music_time);

        void handle_raw_event(const Input::RawEvent& raw_event, const sf::Time& music_time);
        void handle_mouse_click(const sf::Event::MouseButtonEvent& mouse_button_event, const sf::Time& music_time);
        void handle_button_event(const Input::ButtonEvent& button_event, const sf::Time& music_time);
        void handle_button_press(const Input::Button& button, const sf::Time& music_time);
        void handle_button_release(const Input::Button& button, const sf::Time& music_time);

        const Data::SongDifficulty& song_selection;
        const Data::Chart chart;
        const Resources::Marker& marker;
        const Resources::LNMarker& ln_marker;
        std::unique_ptr<AbstractMusic> music;
        Drawables::GradedDensityGraph graded_density_graph;
        std::array<sf::Vertex, 4> cursor;
        // maps music time to [0, 1]
        Toolkit::AffineTransform<float> music_time_to_progression;

        std::deque<Data::GradedNote> notes;
        std::deque<std::reference_wrapper<Data::GradedNote>> visible_notes;
        void miss_old_notes(const sf::Time& music_time);
        void release_finished_longs(const sf::Time& music_time);
        // Performs passive grading actions (misses and long note releases)
        // Then update the visible notes
        void update_visible_notes(const sf::Time& music_time);

        sf::RenderTexture ln_tail_layer;
        sf::RenderTexture marker_layer;

        Data::ClassicScore score;
        std::size_t combo = 0;

        std::atomic<bool> song_finished = false;

        TimedEventsQueue events_queue;

        bool display_black_bars = true;
    };

    Toolkit::AffineTransform<float> get_music_time_to_progression_transform(const Data::SongDifficulty& song_selection);
    
    struct cmpAtomicNotes {
        bool operator()(const std::atomic<Data::GradedNote>& a, const std::atomic<Data::GradedNote>& b) {
            return a.load().timing < b.load().timing;
        }
    };
}

