#pragma once

#include <map>

#include <SFML/Window.hpp>

#include "../../Data/SongList.hpp"
#include "../../Data/Chart.hpp"
#include "../../Data/KeyMapping.hpp"
#include "Ribbon.hpp"
#include "Resources.hpp"

namespace MusicSelect {

    // The music select screen is created only once
    // it loads a cache of available songs in the song_list attribute
    class Screen {
    public:
        Screen(const Data::SongList& t_song_list);
        void select_chart(sf::RenderWindow& window);

    private:
        // Data
        const Data::SongList& song_list;

        // Resources
        Resources resources;

        // State
        Ribbon ribbon;
        std::optional<std::reference_wrapper<SongPanel>> selected_panel;
    
        KeyMapping key_mapping;
        void handle_key(const sf::Event::KeyEvent& key_event);
    };
}
