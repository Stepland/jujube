#pragma once

#include <optional>

#include <SFML/Graphics.hpp>

#include "../../Drawables/GradedDensityGraph.hpp"
#include "../../Data/GradedNote.hpp"
#include "../../Data/Score.hpp"
#include "../../Input/Events.hpp"
#include "../../Toolkit/Debuggable.hpp"
#include "Resources.hpp"


namespace Results {
    class Screen : public HoldsResources, public Toolkit::Debuggable {
    public:
        Screen(
            Drawables::GradedDensityGraph& t_graded_density_graph,
            const Data::SongDifficulty& t_song_selection,
            const Data::AbstractScore& t_score,
            ScreenResources& t_resources
        );
        void display(sf::RenderWindow& window);
    private:
        Drawables::GradedDensityGraph graded_density_graph;
        const Data::SongDifficulty& song_selection;
        const Data::Chart chart;
        const Data::AbstractScore& score;
        bool should_exit = false;

        // converts a key press (keyboard or joystick) into a button press
        void handle_key_press(const Input::Key& key);

        // converts a mouse click into a button press
        std::optional<Input::Button> button_from_position(sf::Vector2i mouse_position);
        void handle_mouse_click(const sf::Event::MouseButtonEvent& mouse_button_event);
        
        void handle_button_press(const Input::Button& button);
    };
}
