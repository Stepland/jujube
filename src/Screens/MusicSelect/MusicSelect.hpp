#pragma once

#include <map>

#include <SFML/Window.hpp>

#include "../../Data/SongList.hpp"
#include "../../Data/Chart.hpp"
#include "../../Data/KeyMaping.hpp"
#include "State.hpp"
#include "Resources.hpp"

namespace MusicSelect {

    // The music select screen is created only once
    // it loads a cache of available songs in the song_list attribute
    class Screen {
    public:
        Screen(const Data::SongList& t_song_list);
        Chart& select_chart(sf::Window& window);
    private:
        const Data::SongList& song_list;
        Resources resources;
        State state;
        KeyMaping key_mapping;
        void handle_key(const sf::Event::KeyEvent& key_event);
    };
};
