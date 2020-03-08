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

MusicSelect::Screen::Screen(const Data::SongList& t_song_list, SharedResources& t_resources) :
    HoldsSharedResources(t_resources),
    song_list(t_song_list),
    ribbon(PanelLayout::title_sort(t_song_list, t_resources), t_resources),
    song_info(t_resources),
    main_option_page(t_resources),
    options_button(t_resources),
    start_button(t_resources),
    black_frame(t_resources.preferences)
{
    panel_filter.setFillColor(sf::Color(0,0,0,200));
    std::cout << "loaded MusicSelect::Screen" << std::endl;
}

void MusicSelect::Screen::select_chart(sf::RenderWindow& window) {
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);
    bool chart_selected = false;
    sf::Clock imguiClock;
    ribbon.setPosition(get_ribbon_x(), get_ribbon_y());
    resources.button_highlight.setPosition(get_ribbon_x(), get_ribbon_y());
    panel_filter.setSize(sf::Vector2f{window.getSize()});
    options_button.setPosition(get_ribbon_x()+2.f*get_panel_step(), get_ribbon_y()+3.f*get_panel_step());
    start_button.setPosition(get_ribbon_x()+3.f*get_panel_step(), get_ribbon_y()+3.f*get_panel_step());
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
                resources.preferences.screen.height = event.size.height;
                resources.preferences.screen.width = event.size.width;
                ribbon.setPosition(get_ribbon_x(), get_ribbon_y());
                resources.button_highlight.setPosition(get_ribbon_x(), get_ribbon_y());
                panel_filter.setSize(sf::Vector2f{window.getSize()});
                if (not resources.options_state.empty()) {
                    resources.options_state.back().get().update();
                }
                options_button.setPosition(get_ribbon_x()+2.f*get_panel_step(), get_ribbon_y()+3.f*get_panel_step());
                start_button.setPosition(get_ribbon_x()+3.f*get_panel_step(), get_ribbon_y()+3.f*get_panel_step());
                break;
            default:
                break;
            }
        }
        ImGui::SFML::Update(window, imguiClock.restart());
        window.clear(sf::Color(7, 23, 53));
        window.draw(ribbon);
        window.draw(options_button);
        window.draw(start_button);
        window.draw(song_info);
        if (not resources.options_state.empty()) {
            window.draw(panel_filter);
            window.draw(resources.options_state.back());
            if (resources.options_state.back().get().should_exit()) {
                resources.options_state.back().get().exit();
                resources.options_state.pop_back();
                if (not resources.options_state.empty()) {
                    resources.options_state.back().get().update();
                }
            }
        }
        window.draw(resources.button_highlight);
        window.draw(black_frame);
        ribbon.draw_debug();
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
                    ImGui::Text("%s", std::to_string(resources.preferences.screen.width).c_str());
                    ImGui::TextUnformatted("height : "); ImGui::SameLine();
                    ImGui::Text("%s", std::to_string(resources.preferences.screen.height).c_str());
                    ImGui::TextUnformatted("fullscreen : "); ImGui::SameLine();
                    ImGui::Text("%s", resources.preferences.screen.fullscreen ? "true" : "false");
                    ImGui::TreePop();
                }
                if (ImGui::TreeNode("layout")) {
                    
                    ImGui::TreePop();
                }
                if (ImGui::TreeNode("options")) {
                    ImGui::TextUnformatted("marker : "); ImGui::SameLine();
                    ImGui::Text("%s", resources.preferences.options.marker.c_str());
                    ImGui::TreePop();
                }
            }
            if (ImGui::CollapsingHeader("Options Menu Stack")) {
                if (resources.options_state.empty()) {
                    ImGui::TextUnformatted("- empty -");
                } else {
                    for (auto &&i : resources.options_state) {
                        ImGui::Text("%s", typeid(i.get()).name());
                    }
                }
            }
        }
        ImGui::End();
    }
}

void MusicSelect::Screen::handle_key_press(const sf::Event::KeyEvent& key_event) {
    // Option Menu takes raw input for potential remapping of keys
    bool output_used = false;
    if (not resources.options_state.empty()) {
        // Safety measure, pressing escape will alway pop the menu page
        if (key_event.code == sf::Keyboard::Escape) {
            resources.options_state.back().get().exit();
            resources.options_state.pop_back();
            if (not resources.options_state.empty()) {
                resources.options_state.back().get().update();
            }
            output_used = true;
        } else {
            output_used = resources.options_state.back().get().handle_raw_input(key_event);
        }
    }
    if (output_used) {
        return;
    }
    auto button = resources.preferences.key_mapping.key_to_button(key_event.code);
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
        static_cast<int>(get_ribbon_x()),
        static_cast<int>(get_ribbon_y())
    };
    auto panels_area_size = static_cast<int>(get_panel_size() * 4.f + get_panel_spacing() * 3.f);
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
    resources.button_highlight.button_pressed(button);
    auto button_index = Data::button_to_index(button);
    if (button_index < 14) {
        ribbon.click_on(button);
    } else {
        switch (button) {
        case Data::Button::B15: // Options Menu
            if (resources.options_state.empty()) {
                resources.options_state.push_back(main_option_page);
                resources.options_state.back().get().update();
            } else {
                resources.options_state.back().get().exit();
                resources.options_state.pop_back();
                if (not resources.options_state.empty()) {
                    resources.options_state.back().get().update();
                }
            }
            break;
        default:
            break;
        }
    }
}
