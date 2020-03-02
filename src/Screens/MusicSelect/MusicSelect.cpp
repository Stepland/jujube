#include "MusicSelect.hpp"

#include "imgui/imgui.h"
#include "imgui-sfml/imgui-SFML.h"

#include <iostream>

#include "../../Data/Buttons.hpp"
#include "../../Data/KeyMapping.hpp"
#include "PanelLayout.hpp"

MusicSelect::Screen::Screen(
    const Data::SongList& t_song_list,
    Data::Preferences& t_preferences,
    const Resources::Markers& t_markers
) :
    song_list(t_song_list),
    resources(t_preferences),
    markers(t_markers),
    ribbon(PanelLayout::title_sort(t_song_list, resources), resources),
    song_info(resources),
    selected_panel(),
    button_highlight(resources),
    black_frame(t_preferences),
    key_mapping()
{
    std::cout << "loaded MusicSelect::Screen" << std::endl;
}

void MusicSelect::Screen::select_chart(sf::RenderWindow& window) {
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);
    bool chart_selected = false;
    sf::Clock imguiClock;
    ribbon.setPosition(
        resources.m_preferences.layout.ribbon_x*resources.m_preferences.screen.width,
        resources.m_preferences.layout.ribbon_y*resources.m_preferences.screen.width
    );
    button_highlight.setPosition(
        resources.m_preferences.layout.ribbon_x*resources.m_preferences.screen.width,
        resources.m_preferences.layout.ribbon_y*resources.m_preferences.screen.width
    );
    while ((not chart_selected) and window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            switch (event.type) {
            case sf::Event::KeyPressed:
                handle_key_press(event.key);
                break;
            case sf::Event::MouseButtonPressed:
                handle_mouse_click(event.mouseButton);
                break;
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::Resized:
                // update the view to the new size of the window
                window.setView(sf::View({0, 0, static_cast<float>(event.size.width), static_cast<float>(event.size.height)}));
                resources.m_preferences.screen.height = event.size.height;
                resources.m_preferences.screen.width = event.size.width;
                ribbon.setPosition(
                    resources.m_preferences.layout.ribbon_x*resources.m_preferences.screen.width,
                    resources.m_preferences.layout.ribbon_y*resources.m_preferences.screen.width
                );
                button_highlight.setPosition(
                    resources.m_preferences.layout.ribbon_x*resources.m_preferences.screen.width,
                    resources.m_preferences.layout.ribbon_y*resources.m_preferences.screen.width
                );
                break;
            default:
                break;
            }
        }
        ImGui::SFML::Update(window, imguiClock.restart());
        window.clear(sf::Color(7, 23, 53));
        ribbon.draw_debug();
        window.draw(ribbon);
        window.draw(button_highlight);
        window.draw(song_info);
        window.draw(black_frame);
        ImGui::SFML::Render(window);
        window.display();
        resources.music_preview.update();
    }
    ImGui::SFML::Shutdown();
}

void MusicSelect::Screen::handle_key_press(const sf::Event::KeyEvent& key_event) {
    auto button = key_mapping.key_to_button(key_event.code);
    if (button) {
        press_button(*button);
    } else {
        switch (key_event.code){
        case sf::Keyboard::F12:
            ribbon.debug = not ribbon.debug;
            break;
        default:
            break;
        }
    }
}

void MusicSelect::Screen::handle_mouse_click(const sf::Event::MouseButtonEvent& /*mouse_button_event*/) {
    /*
    if (mouse_button_event.button == sf::Mouse::Left) {
        int clicked_panel_index = (mouse_button_event.x / m_panel_size) + 4 * (mouse_button_event.y / m_panel_size);
        auto button = fromIndex(clicked_panel_index);
        if (button) {
            press_button(*button);
        }
    }
    */
}

void MusicSelect::Screen::press_button(const Data::Button& button) {
    button_highlight.button_pressed(button);
    auto button_index = Data::button_to_index(button);
    if (button_index < 12) {
        ribbon.click_on(button);
    } else {
        switch (button) {
        case Data::Button::B13:
            ribbon.move_left();
            break;
        case Data::Button::B14:
            ribbon.move_right();
        default:
            break;
        }
    }
}
