#include "MusicSelect.hpp"

#include <iostream>

#include "../../Data/KeyMapping.hpp"

MusicSelect::Screen::Screen(const Data::SongList& t_song_list) :
    song_list(t_song_list),
    ribbon(MusicSelect::Ribbon::test_sort())
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

void MusicSelect::Screen::select_chart(sf::RenderWindow& window) {
    
    window.create(sf::VideoMode(600,600), "jujube", sf::Style::None);
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
        // drawing the ribbon
        for (size_t panel = 0; panel < 12; panel++) {
            ribbon.at(panel)->draw(
                *this,
                window,
                sf::FloatRect((panel%4)*150.f, (panel/4)*150.f, 150.f, 150.f)
            );
        }
        window.display();
    }
}

void MusicSelect::Screen::handle_key(const sf::Event::KeyEvent& key_event) {
    auto panel = key_mapping.key_to_button(key_event.code);
    if (panel) {
        if (Button::B2 <= *panel and *panel <= Button::B12) {
            ribbon.at()
        }
    }
}
