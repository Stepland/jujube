#include "Results.hpp"

#include <imgui/imgui.h>
#include <imgui-sfml/imgui-SFML.h>
#include <SFML/System/Clock.hpp>

namespace Results {
    Screen::Screen(
        Drawables::GradedDensityGraph& t_graded_density_graph,
        const Data::SongDifficulty& t_song_selection,
        const Data::AbstractScore& t_score,
        ScreenResources& t_resources
    ) :
        HoldsResources(t_resources),
        graded_density_graph(t_graded_density_graph),
        song_selection(t_song_selection),
        chart(*t_song_selection.get_chart()),
        score(t_score)
    {
    }

    void Screen::display(sf::RenderWindow& window) {
        window.setKeyRepeatEnabled(true);
        window.setActive(true);
        sf::Clock imgui_clock;
        auto final_score = score.get_final_score();
        auto final_rating = score.get_rating();
        while ((not should_exit) and window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                ImGui::SFML::ProcessEvent(event);
                switch (event.type) {
                case sf::Event::KeyPressed:
                    handle_key_press(event.key.code);
                    break;
                case sf::Event::MouseButtonPressed:
                    handle_mouse_click(event.mouseButton);
                    break;
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::Resized:
                    window.setView(
                        sf::View({0, 0, static_cast<float>(event.size.width), static_cast<float>(event.size.height)})
                    );
                    preferences.screen.video_mode.height = event.size.height;
                    preferences.screen.video_mode.width = event.size.width;
                    shared.button_highlight.setPosition(get_ribbon_x(), get_ribbon_y());
                    break;
                default:
                    break;
                }
            }
            ImGui::SFML::Update(window, imgui_clock.restart());
            window.clear(sf::Color(7, 23, 53));

            // White line under the density graph
            sf::RectangleShape line{{get_screen_width()*1.1f,2.f/768.f*get_screen_width()}};
            Toolkit::set_origin_normalized(line, 0.5f, 0.5f);
            line.setFillColor(sf::Color::White);
            line.setPosition(get_screen_width()*0.5f,425.f/768.f*get_screen_width());
            window.draw(line);

            // Density Graph
            Toolkit::set_local_origin_normalized(graded_density_graph, 0.5f, 1.f);
            graded_density_graph.setScale(get_screen_width()/768.f, get_screen_width()/768.f);
            graded_density_graph.setPosition(get_screen_width()*0.5f,423.f/768.f*get_screen_width());
            window.draw(graded_density_graph);

            // Draw score
            sf::Text score_text;
            score_text.setFont(shared.fallback_font.black);
            score_text.setFillColor(sf::Color(29, 98, 226));
            score_text.setString(std::to_string(final_score));
            score_text.setCharacterSize(static_cast<unsigned int>(45.f/768.f*get_screen_width()));
            Toolkit::set_local_origin_normalized(score_text, 1.f, 1.f);
            score_text.setPosition(
                500.f/768.f*get_screen_width(),
                370.f/768.f*get_screen_width()
            );
            window.draw(score_text);

            // Draw Rating
            sf::Text rating_text;
            rating_text.setFont(shared.fallback_font.black);
            rating_text.setFillColor(sf::Color(29, 98, 226));
            rating_text.setString(Data::rating_to_string.at(final_rating));
            rating_text.setCharacterSize(static_cast<unsigned int>(0.5f*get_panel_size()));
            Toolkit::set_local_origin_normalized(rating_text, 0.5f, 0.5f);
            rating_text.setPosition(
                get_ribbon_x()+2.f*get_panel_step()+0.5*get_panel_size(),
                get_ribbon_y()+2.f*get_panel_step()+0.5*get_panel_size()
            );
            window.draw(rating_text);

            // Draw song info
            auto song_title = song_selection.song.title;
            if (not song_title.empty()) {
                sf::Text song_title_label{
                    sf::String::fromUtf8(song_title.begin(), song_title.end()),
                    shared.fallback_font.medium,
                    static_cast<unsigned int>(scale(20.f))
                };
                Toolkit::set_local_origin_normalized(song_title_label, 0.f, 1.f);
                song_title_label.setFillColor(sf::Color::White);
                song_title_label.setPosition(scale(440.f), scale(40.f));
                window.draw(song_title_label);
            }
            auto song_artist = song_selection.song.artist;
            if (not song_artist.empty()) {
                sf::Text song_artist_label{
                    sf::String::fromUtf8(song_artist.begin(), song_artist.end()),
                    shared.fallback_font.medium,
                    static_cast<unsigned int>(scale(12.f))
                };
                song_artist_label.setStyle(sf::Text::Italic);
                song_artist_label.setFillColor(sf::Color::White);
                song_artist_label.setPosition(scale(440.f), scale(45.f));
                window.draw(song_artist_label);
            }
            sf::Text level_label{
                "LEVEL:",
                shared.fallback_font.medium,
                static_cast<unsigned int>(scale(10.f))
            };
            Toolkit::set_origin_normalized(level_label, 1.f, 1.f);
            level_label.setPosition(scale(322.f), scale(35.f));
            level_label.setFillColor(sf::Color::White);
            window.draw(level_label);
            
            sf::Text level_number_label{
                std::to_string(chart.level),
                shared.fallback_font.black,
                static_cast<unsigned int>(scale(35.f))
            };
            Toolkit::set_origin_normalized(level_number_label, 0.5f, 0.f);
            level_number_label.setPosition(scale(351.f), scale(24.f));
            level_number_label.setFillColor(sf::Color::White);
            window.draw(level_number_label);

            std::string full_difficulty = song_selection.difficulty;
            if (full_difficulty == "BSC") {
                full_difficulty = "BASIC";
            } else if (full_difficulty == "ADV") {
                full_difficulty = "ADVANCED";
            } else if (full_difficulty == "EXT") {
                full_difficulty = "EXTREME";
            }

            sf::Text chart_label{
                sf::String::fromUtf8(full_difficulty.begin(), full_difficulty.end()),
                shared.fallback_font.medium,
                static_cast<unsigned int>(scale(16.f))
            };
            Toolkit::set_local_origin_normalized(chart_label, 1.f, 1.f);
            chart_label.setPosition(scale(322.f), scale(55.f));
            chart_label.setFillColor(shared.get_chart_color(song_selection.difficulty));
            window.draw(chart_label);

            shared.button_highlight.update();
            window.draw(shared.button_highlight);
            
            window.draw(shared.black_frame);
            
            if (debug) {
                draw_debug();
            }
            ImGui::SFML::Render(window);
            window.display();
        }
    }

    void Screen::handle_key_press(const Input::Key& key) {
        // Option Menu takes raw input for potential remapping of keys
        auto button = shared.preferences.key_mapping.key_to_button(key);
        if (button) {
            handle_button_press(*button);
        } else if (auto keyboard_key = std::get_if<sf::Keyboard::Key>(&key)) {
            switch (*keyboard_key) {
            case sf::Keyboard::F12:
                debug = not debug;
                break;
            default:
                break;
            }
        }
    }

    std::optional<Input::Button> Screen::button_from_position(sf::Vector2i mouse_position) {
        sf::Vector2i ribbon_origin{
            static_cast<int>(get_ribbon_x()),
            static_cast<int>(get_ribbon_y())
        };
        auto panels_area_size = static_cast<int>(get_panel_size() * 4.f + get_panel_spacing() * 3.f);
        sf::IntRect panels_area{ribbon_origin, sf::Vector2i{panels_area_size, panels_area_size}};
        if (not panels_area.contains(mouse_position)) {
            return {};
        }

        sf::Vector2i relative_mouse_pos = mouse_position - ribbon_origin;
        int clicked_panel_index = (
            (relative_mouse_pos.x / (panels_area_size/4))
            + 4 * (relative_mouse_pos.y / (panels_area_size/4))
        );
        if (clicked_panel_index < 0) {
            return {};
        }
        return Input::index_to_button(static_cast<std::size_t>(clicked_panel_index));
    }

    void Screen::handle_mouse_click(const sf::Event::MouseButtonEvent& mouse_event) {
        if (mouse_event.button != sf::Mouse::Button::Left) {
            return;
        }
        if (auto button = button_from_position({mouse_event.x, mouse_event.y})) {
            handle_button_press(*button);
        }
    }

    void Screen::handle_button_press(const Input::Button& button) {
        shared.button_highlight.button_pressed(button);
        switch (button) {
        case Input::Button::B16:
            should_exit = true;
            break;
        default:
            break;
        }
    }
}
