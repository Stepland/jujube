#pragma once

#include <atomic>
#include <deque>
#include <functional>
#include <memory>
#include <tuple>
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
#include "Drawables/Cursor.hpp"
#include "Drawables/Shutter.hpp"

namespace Gameplay {
    struct DetailedScore {
        Drawables::GradedDensityGraph& gdg;
        Data::AbstractScore& score;
    };

    class Screen : public Toolkit::Debuggable, public HoldsResources {
    public:
        explicit Screen(const Data::SongDifficulty& song_selection, ScreenResources& t_resources);
        DetailedScore play_chart(sf::RenderWindow& window);
    private:
        void draw_debug() override;
        void render(sf::RenderWindow& window);
        // Draw a normal note on marker_layer
        void draw_tap_note(const Data::GradedNote& note, const sf::Time& music_time);
        // Draw the long note tail on ln_tail_layer and its markers on marker_layer
        void draw_long_note(const Data::GradedNote& note, const sf::Time& music_time);

        void handle_raw_input_event(const Input::RawEvent& raw_event, const sf::Time& music_time);
        
        std::optional<Input::Button> button_from_position(sf::Vector2i position);
        std::optional<Input::Button> last_mouse_clicked_button;
        void handle_mouse_click(const sf::Event::MouseButtonEvent& mouse_event, const sf::Time& music_time);
        void handle_mouse_move(const sf::Event::MouseMoveEvent& mouse_move_event, const sf::Time& music_time);
        void handle_mouse_release(const sf::Event::MouseButtonEvent& mouse_event, const sf::Time& music_time);

        std::unordered_map<unsigned int, std::optional<Input::Button>> last_touched_button;
        void handle_touch_began(const sf::Event::TouchEvent& touch_event, const sf::Time& music_time);
        void handle_touch_moved(const sf::Event::TouchEvent& touch_event, const sf::Time& music_time);
        void handle_touch_ended(const sf::Event::TouchEvent& touch_event, const sf::Time& music_time);
        
        void handle_button_event(const Input::ButtonEvent& button_event, const sf::Time& music_time);
        void handle_button_press(const Input::Button& button, const sf::Time& music_time);
        void handle_button_release(const Input::Button& button, const sf::Time& music_time);

        const Data::SongDifficulty& song_selection;
        const Data::Chart chart;
        const Resources::Marker& marker;
        const Resources::LNMarker& ln_marker;
        std::unique_ptr<AbstractMusic> music;
        Drawables::GradedDensityGraph graded_density_graph;
        Drawables::Cursor cursor;
        Drawables::Shutter shutter;

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
}

