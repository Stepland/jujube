#include "MusicSelect.hpp"

#include <functional>
#include <iostream>

#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>
#include <imgui-sfml/imgui-SFML.h>

#include "../../Data/Buttons.hpp"
#include "../../Data/KeyMapping.hpp"
#include "../../Toolkit/NormalizedOrigin.hpp"
#include "Panels/Panel.hpp"
#include "PanelLayout.hpp"

MusicSelect::Screen::Screen(
    const Data::SongList& t_song_list,
    Data::Preferences& t_preferences,
    const Resources::Markers& t_markers
) :
    song_list(t_song_list),
    resources(t_preferences, t_markers),
    markers(t_markers),
    ribbon(PanelLayout::title_sort(t_song_list, resources), resources),
    song_info(resources),
    button_highlight(resources),
    main_option_page(resources),
    black_frame(t_preferences),
    key_mapping()
{
    panel_filter.setSize(sf::Vector2f(
        resources.m_preferences.layout.panel_step()*resources.m_preferences.screen.width*4.f,
        resources.m_preferences.layout.panel_step()*resources.m_preferences.screen.width*4.f
    ));
    Toolkit::set_origin_normalized(panel_filter, 0.5f, 0.5f);
    panel_filter.setFillColor(sf::Color(0,0,0,128));
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
    panel_filter.setPosition(
        sf::Vector2f{resources.m_preferences.layout.ribbon_x + (
            3.f*resources.m_preferences.layout.panel_spacing + 
            4.f*resources.m_preferences.layout.panel_size
        ) / 2.f,
        resources.m_preferences.layout.ribbon_y + (
            3.f*resources.m_preferences.layout.panel_spacing + 
            4.f*resources.m_preferences.layout.panel_size
        ) / 2.f}*static_cast<float>(resources.m_preferences.screen.width)
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
                panel_filter.setPosition(
                    sf::Vector2f{resources.m_preferences.layout.ribbon_x + (
                        3.f*resources.m_preferences.layout.panel_spacing + 
                        4.f*resources.m_preferences.layout.panel_size
                    ) / 2.f,
                    resources.m_preferences.layout.ribbon_y + (
                        3.f*resources.m_preferences.layout.panel_spacing + 
                        4.f*resources.m_preferences.layout.panel_size
                    ) / 2.f}*static_cast<float>(resources.m_preferences.screen.width)
                );
                if (not resources.options_state.empty()) {
                    resources.options_state.top().get().update();
                }
                break;
            default:
                break;
            }
        }
        ImGui::SFML::Update(window, imguiClock.restart());
        window.clear(sf::Color(7, 23, 53));
        ribbon.draw_debug();
        window.draw(ribbon);
        if (not resources.options_state.empty()) {
            window.draw(panel_filter);
            window.draw(resources.options_state.top());
        }
        window.draw(button_highlight);
        window.draw(song_info);
        window.draw(black_frame);
        draw_debug();
        ImGui::SFML::Render(window);
        window.display();
        resources.music_preview.update();
    }
    ImGui::SFML::Shutdown();
}

void MusicSelect::Screen::draw_debug() {
    if (debug) {
        if (ImGui::Begin("MusicSelect::Screen")) {
            if (ImGui::CollapsingHeader("Preferences")) {
                if (ImGui::TreeNode("screen")) {
                    ImGui::TextUnformatted("width : "); ImGui::SameLine();
                    ImGui::Text("%s", std::to_string(resources.m_preferences.screen.width).c_str());
                    ImGui::TextUnformatted("height : "); ImGui::SameLine();
                    ImGui::Text("%s", std::to_string(resources.m_preferences.screen.height).c_str());
                    ImGui::TextUnformatted("fullscreen : "); ImGui::SameLine();
                    ImGui::Text("%s", resources.m_preferences.screen.fullscreen ? "true" : "false");
                    ImGui::TreePop();
                }
                if (ImGui::TreeNode("layout")) {
                    
                    ImGui::TreePop();
                }
                if (ImGui::TreeNode("options")) {
                    ImGui::TextUnformatted("marker : "); ImGui::SameLine();
                    ImGui::Text("%s", resources.m_preferences.options.marker.c_str());
                    ImGui::TreePop();
                }
            }
        }
        ImGui::End();
    }
}

void MusicSelect::Screen::handle_key_press(const sf::Event::KeyEvent& key_event) {
    auto button = key_mapping.key_to_button(key_event.code);
    if (button) {
        press_button(*button);
    } else {
        switch (key_event.code){
        case sf::Keyboard::F12:
            ribbon.debug = not ribbon.debug;
            debug = not debug;
            break;
        default:
            break;
        }
    }
}

void MusicSelect::Screen::handle_mouse_click(const sf::Event::MouseButtonEvent& mouse_button_event) {
    if (mouse_button_event.button != sf::Mouse::Left) {
        return;
    }

    sf::Vector2i mouse_position{mouse_button_event.x, mouse_button_event.y};
    sf::Vector2i ribbon_origin{
        static_cast<int>(resources.get_ribbon_x()),
        static_cast<int>(resources.get_ribbon_y())
    };
    auto panels_area_size = static_cast<int>(
        resources.get_panel_size() * 4.f + resources.get_panel_spacing() * 3.f
    );
    sf::IntRect panels_area{ribbon_origin, sf::Vector2i{panels_area_size, panels_area_size}};
    if (not panels_area.contains(mouse_position)) {
        return;
    }

    sf::Vector2i relative_mouse_pos = mouse_position - ribbon_origin;
    int clicked_panel_index = (
        (relative_mouse_pos.x / (panels_area_size/4))
        + 4 * (relative_mouse_pos.y / (panels_area_size/4))
    );
    if (clicked_panel_index < 0) {
        return;
    }
    auto button = Data::index_to_button(static_cast<std::size_t>(clicked_panel_index));
    if (button) {
        press_button(*button);
    }
}

void MusicSelect::Screen::press_button(const Data::Button& button) {
    button_highlight.button_pressed(button);
    auto button_index = Data::button_to_index(button);
    // Are we displaying the options menu ?
    if (not resources.options_state.empty()) {
        if (button_index < 14) {
            resources.options_state.top().get().click(button);
        } else {
            if (button == Data::Button::B15) {
                resources.options_state.pop();
                if (not resources.options_state.empty()) {
                    resources.options_state.top().get().update();
                }
            }
        }
    } else {
        if (button_index < 12) {
            ribbon.click_on(button);
        } else {
            switch (button) {
            case Data::Button::B13: // Left Arrow
                ribbon.move_left();
                break;
            case Data::Button::B14: // Right Arrow
                ribbon.move_right();
                break;
            case Data::Button::B15: // Options Menu
                resources.options_state.push(main_option_page);
                resources.options_state.top().get().update();
                break;
            default:
                break;
            }
        }
    }
}
