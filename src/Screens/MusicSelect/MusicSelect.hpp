#pragma once

#include <map>
#include <optional>
#include <stack>

#include <SFML/Window.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "../../Data/Song.hpp"
#include "../../Data/Chart.hpp"
#include "../../Input/KeyMapping.hpp"
#include "../../Drawables/BlackFrame.hpp"
#include "../../Resources/Marker.hpp"
#include "../../Toolkit/AffineTransform.hpp"
#include "../../Toolkit/Debuggable.hpp"
#include "Ribbon.hpp"
#include "SongInfo.hpp"
#include "Resources.hpp"
#include "Options/OptionPage.hpp"
#include "Drawables/ControlPanels.hpp"

namespace MusicSelect {

    class SongPanel;
    // The music select screen is created only once
    // it loads a cache of available songs in the song_list attribute
    class Screen : public Toolkit::Debuggable, public HoldsResources {
    public:
        Screen(const Data::SongList& t_song_list, ScreenResources& t_resources);
        std::optional<Data::SongDifficulty> select_chart(sf::RenderWindow& window);
        void draw_debug(sf::RenderWindow& window);
    private:
        const Data::SongList song_list;

        Ribbon ribbon;
        SongInfo song_info;
        MainOptionPage main_option_page;
        OptionsButton options_button;
        StartButton start_button;
        bool chart_selected = false;

        sf::RectangleShape panel_filter;
    
        // converts a key press into a button press
        void handle_key_press(const sf::Event::KeyEvent& key_event, sf::RenderWindow& window);
        // converts a mouse click into a button press
        void handle_mouse_click(const sf::Event::MouseButtonEvent& mouse_button_event);
        // chooses what happens for each button
        void press_button(Input::Button button);

        void cycle_display_styles(sf::RenderWindow& window);

        void update_view(sf::RenderWindow& window, sf::View view);
    };
}
