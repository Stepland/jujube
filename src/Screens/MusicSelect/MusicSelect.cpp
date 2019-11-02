#include "MusicSelect.hpp"

#include <iostream>

#include "../../Data/KeyMaping.hpp"

MusicSelect::Screen::Screen(const Data::SongList& t_song_list) :
    song_list(t_song_list),
    resources(),
    state(t_song_list)
{
    for (const auto& song : song_list.songs) {
        if (song.cover) {
            try {
                resources.cover_previews.emplace_back(song.cover.value());
            } catch(const std::exception& e) {
                std::cerr << e.what() << '\n';
            }
        }
    }
}

Chart& MusicSelect::Screen::select_chart(sf::Window& window) {
    bool chart_selected = false;
    while (not chart_selected) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::KeyPressed:
                handle_key(event.key);
                break;
            default:
                break;
            }
        }
    }
}

void MusicSelect::Screen::handle_key(const sf::Event::KeyEvent& key_event) {
    auto panel = key_mapping.key_to_panel(key_event.code);
    if (panel) {
        if (PanelEnum::P2 <= *panel and *panel <= PanelEnum::P12) {
            auto coord = toCoord(*panel);
            auto ribbon_size = state.ribbon.get_layout().size();
            state
                .ribbon
                .get_layout()
                .at((state.ribbon_position + coord.x) % ribbon_size)
                .at(coord.y)
                ->click(state);
        }
    }
}
