#include "MusicSelect.hpp"

#include <iostream>

#include "../../Data/KeyMapping.hpp"

MusicSelect::Screen::Screen(const Data::SongList& t_song_list) :
    song_list(t_song_list),
    button_highlight(panel_size),
    ribbon(resources)
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
    ribbon.test2_sort();
}

void MusicSelect::Screen::select_chart(sf::RenderWindow& window) {
    
    window.create(sf::VideoMode(panel_size*4, panel_size*4), "jujube", sf::Style::Titlebar);
    window.setFramerateLimit(60);
    bool chart_selected = false;
    while (not chart_selected) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::KeyPressed:
                handle_key_press(event.key);
                break;
            case sf::Event::MouseButtonPressed:
                handle_mouse_click(event.mouseButton);
            default:
                break;
            }
        }
        window.draw(ribbon);
        window.draw(button_highlight);
        window.display();
        window.clear(sf::Color::Black);
    }
}

void MusicSelect::Screen::handle_key_press(const sf::Event::KeyEvent& key_event) {
    auto button = key_mapping.key_to_button(key_event.code);
    if (button) {
        press_button(*button);
    }
}

void MusicSelect::Screen::handle_mouse_click(const sf::Event::MouseButtonEvent& mouse_button_event) {
    if (mouse_button_event.button == sf::Mouse::Left) {
        int clicked_panel_index = (mouse_button_event.x / panel_size) + 4 * (mouse_button_event.y / panel_size);
        auto button = fromIndex(clicked_panel_index);
        if (button) {
            press_button(*button);
        }
    }
}

void MusicSelect::Screen::press_button(const Button& button) {
    button_highlight.button_pressed(button);
    auto index = toIndex(button);
    if (index < 12) {
        ribbon.at(index)->click(*this);
    } else {
        switch (button) {
        case Button::B13:
            ribbon.move_left();
            break;
        case Button::B14:
            ribbon.move_right();
        default:
            break;
        }
    }
}
