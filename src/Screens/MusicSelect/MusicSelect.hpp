#pragma once

#include <map>
#include <stack>

#include <SFML/Window.hpp>

#include "../../Data/Song.hpp"
#include "../../Data/Chart.hpp"
#include "../../Data/KeyMapping.hpp"
#include "../../Drawables/BlackFrame.hpp"
#include "../../Resources/Marker.hpp"
#include "../../Toolkit/AffineTransform.hpp"
#include "SongListRibbon.hpp"
#include "SongInfo.hpp"
#include "SharedResources.hpp"
#include "ButtonHighlight.hpp"
#include "OptionMenu.hpp"

namespace MusicSelect {

    // The music select screen is created only once
    // it loads a cache of available songs in the song_list attribute
    class Screen {
    public:
        Screen(
            const Data::SongList& t_song_list,
            Data::Preferences& t_preferences,
            const Resources::Markers& t_markers
        );
        void select_chart(sf::RenderWindow& window);

    private:
        // Data
        const Data::SongList song_list;

        // Resources
        SharedResources resources;
        Resources::Markers markers;

        // State
        SongListRibbon ribbon;
        SongInfo song_info;
        std::optional<std::reference_wrapper<SongPanel>> selected_panel;
        ButtonHighlight button_highlight;
        std::stack<OptionMenu> options_state;

        Drawables::BlackFrame black_frame;
    
        Data::KeyMapping key_mapping;
        // converts a key press into a button press
        void handle_key_press(const sf::Event::KeyEvent& key_event);
        // converts a mouse click into a button press
        void handle_mouse_click(const sf::Event::MouseButtonEvent& mouse_button_event);
        // chooses what happens for each button
        void press_button(const Data::Button& button);
    };
}
