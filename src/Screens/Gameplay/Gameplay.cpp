#include "Gameplay.hpp"

#include <algorithm>
#include <chrono>
#include <optional>
#include <thread>
#include <utility>

#include <imgui/imgui.h>
#include <imgui-sfml/imgui-SFML.h>
#include <SFML/Graphics.hpp>

#include "../../Input/Buttons.hpp"
#include "../../Toolkit/SFMLHelpers.hpp"
#include "PreciseMusic.hpp"
#include "Silence.hpp"

namespace Gameplay {
    Screen::Screen(const Data::SongDifficulty& t_song_selection, ScreenResources& t_resources) :
        HoldsResources(t_resources),
        song_selection(t_song_selection),
        chart(*t_song_selection.song.get_chart(t_song_selection.difficulty)),
        marker(t_resources.shared.get_selected_marker()),
        score(t_song_selection.song.get_chart(t_song_selection.difficulty)->notes)
    {
        for (auto&& note : chart.notes) {
            notes.emplace_back(Data::GradedNote{note});
        }
        auto music_path = song_selection.song.full_audio_path();
        if (music_path) {
            music = std::make_unique<PreciseMusic>(music_path->string());
        } else {
            music = std::make_unique<Silence>(chart.get_duration_based_on_notes());
        }
    }

    void Screen::play_chart(sf::RenderWindow& window) {
        window.setActive(false);
        std::thread render_thread(&Screen::render, this, std::ref(window));
        while ((not song_finished) and window.isOpen()) {
            auto music_time = music->getPlayingOffset();
            sf::Event event;
            while (window.pollEvent(event)) {
                ImGui::SFML::ProcessEvent(event);
                switch (event.type) {
                case sf::Event::KeyPressed:
                    handle_raw_event(event.key.code, music_time);
                    break;
                case sf::Event::JoystickButtonPressed:
                    handle_raw_event(event.joystickButton, music_time);
                    break;
                case sf::Event::MouseButtonPressed:
                    handle_mouse_click(event.mouseButton, music_time);
                    break;
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::Resized:
                    // update the view to the new size of the window
                    window.setView(sf::View({0, 0, static_cast<float>(event.size.width), static_cast<float>(event.size.height)}));
                    shared.preferences.screen.height = event.size.height;
                    shared.preferences.screen.width = event.size.width;
                    shared.button_highlight.setPosition(get_ribbon_x(), get_ribbon_y());
                    break;
                default:
                    break;
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        if (render_thread.joinable()) {
            render_thread.join();
        }
    }
    
    void Screen::render(sf::RenderWindow& window) {
        window.setActive(true);
        sf::Clock imguiClock;
        music->play();
        while ((not song_finished) and window.isOpen()) {
            song_finished = music->getStatus() == sf::Music::Stopped;
            ImGui::SFML::Update(window, imguiClock.restart());
            auto music_time = music->getPlayingOffset();
            update_note_index(music_time);
            window.clear(sf::Color(7, 23, 53));

            // Draw song info
            // Cover is 40x40 @ (384,20)
            if (song_selection.song.cover) {
                auto cover = shared.covers.get(*song_selection.song.full_cover_path());
                if (cover) {
                    sf::Sprite cover_sprite{*cover->texture};
                    auto cover_size = 40.f/768.f*get_screen_width();
                    Toolkit::set_size_from_local_bounds(cover_sprite, cover_size, cover_size);
                    cover_sprite.setPosition(
                        384.f/768.f*get_screen_width(),
                        20.f/768.f*get_screen_width()
                    );
                    window.draw(cover_sprite);
                }
            } 


            // Draw Combo
            auto current_combo = combo.load();
            if (current_combo >= 4) {
                sf::Text combo_text;
                combo_text.setFont(shared.fallback_font.black);
                combo_text.setFillColor(sf::Color(18, 59, 135));
                combo_text.setString(std::to_string(current_combo));
                combo_text.setCharacterSize(static_cast<unsigned int>(1.5*get_panel_step()));
                Toolkit::set_local_origin_normalized(combo_text, 0.5f, 0.5f);
                combo_text.setPosition(
                    get_ribbon_x()+get_ribbon_size()*0.5f,
                    get_ribbon_y()+get_ribbon_size()*0.5f
                );
                window.draw(combo_text);
            }

            // Draw score
            auto current_score = score.get_score();
            sf::Text score_text;
            score_text.setFont(shared.fallback_font.black);
            score_text.setFillColor(sf::Color(29, 98, 226));
            score_text.setString(std::to_string(current_score));
            score_text.setCharacterSize(static_cast<unsigned int>(45.f/768.f*get_screen_width()));
            Toolkit::set_local_origin_normalized(score_text, 1.f, 1.f);
            score_text.setPosition(
                500.f/768.f*get_screen_width(),
                370.f/768.f*get_screen_width()
            );
            window.draw(score_text);

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

            // Draw Notes
            for (auto i = note_index.load(); i < notes.size(); ++i) {
                auto note = notes[i].load();
                std::optional<sf::Sprite> sprite;
                if (note.timed_judgement) {
                    sprite = marker.get_sprite(
                        judgement_to_animation(note.timed_judgement->judgement),
                        music_time-note.timing-note.timed_judgement->delta
                    );
                } else {
                    sprite = marker.get_sprite(
                        Resources::MarkerAnimation::APPROACH,
                        music_time-note.timing
                    );
                }
                if (sprite) {
                    Toolkit::set_size_from_local_bounds(*sprite, get_panel_size(), get_panel_size());
                    auto pos = Input::button_to_coords(note.position);
                    sprite->setPosition(
                        get_ribbon_x()+get_panel_step()*pos.x,
                        get_ribbon_y()+get_panel_step()*pos.y
                    );
                    window.draw(*sprite);
                }
            }
            window.draw(shared.button_highlight);
            window.draw(shared.black_frame);
            if (debug) {
                draw_debug();
            }
            ImGui::SFML::Render(window);
            window.display();
        }
    }

    void Gameplay::Screen::handle_mouse_click(const sf::Event::MouseButtonEvent& mouse_button_event, const sf::Time& music_time) {
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
        auto button = Input::index_to_button(static_cast<std::size_t>(clicked_panel_index));
        if (button) {
            handle_button(*button, music_time);
        }
    }

    void Screen::handle_raw_event(const Input::Event& event, const sf::Time& music_time) {
        auto button = preferences.key_mapping.key_to_button(event);
        if (button) {
            handle_button(*button, music_time);
        } else if (auto key = std::get_if<sf::Keyboard::Key>(&event)) {
            switch (*key) {
            case sf::Keyboard::F12:
                debug = not debug;
                break;
            default:
                break;
            }
        }
    }

    void Screen::handle_button(const Input::Button& button, const sf::Time& music_time) {
        shared.button_highlight.button_pressed(button);
        // Is the music even playing ?
        if (music->getStatus() == sf::SoundSource::Playing) {
            update_note_index(music_time);
            for (auto i = note_index.load(); i < notes.size(); ++i) {
                auto note = notes[i].load();
                // is the note still visible ?
                if (note.timing > music_time + sf::seconds(16.f/30.f)) {
                    break;
                }
                // is it even the right button ?
                if (note.position != button) {
                    continue;
                }
                // has it already been graded ?
                if (note.timed_judgement) {
                    continue;
                }
                auto graded_note = Data::GradedNote{note, music_time-note.timing};
                auto& judgement = graded_note.timed_judgement->judgement;
                score.update(judgement);
                switch (judgement) {
                case Data::Judgement::Perfect:
                case Data::Judgement::Great:
                case Data::Judgement::Good:
                    combo++;
                    break;
                default:
                    combo = 0;
                    break;
                }
                notes[i].store(graded_note);
                break;
            }
        }
    }

    void Screen::update_note_index(const sf::Time& music_time) {
        for (auto i = note_index.load(); i < notes.size(); ++i) {
            auto note = notes[i].load();
            if (note.timing >= music_time - sf::seconds(16.f/30.f)) {
                note_index = i;
                break;
            } else {
                if (not note.timed_judgement) {
                    note.timed_judgement.emplace(sf::Time::Zero, Data::Judgement::Miss);
                    score.update(Data::Judgement::Miss);
                    combo = 0;
                    notes[i].store(note);
                }
            }
        }
    }

    void Screen::draw_debug() {
        if (ImGui::Begin("Gameplay Debug")) {
            ImGui::Text("Combo : %lu", combo.load());
            if (ImGui::TreeNode("Score")) {
                ImGui::Text("Raw           : %d", score.get_score());
                ImGui::Text("Final         : %d", score.get_final_score());
                ImGui::Text("Shutter value : %d", score.shutter);
                if (ImGui::TreeNode("Judgement Counts")) {
                    ImGui::Text("PERFECT : %lu", score.judgement_counts.at(Data::Judgement::Perfect));
                    ImGui::Text("GREAT   : %lu", score.judgement_counts.at(Data::Judgement::Great));
                    ImGui::Text("GOOD    : %lu", score.judgement_counts.at(Data::Judgement::Good));
                    ImGui::Text("POOR    : %lu", score.judgement_counts.at(Data::Judgement::Poor));
                    ImGui::Text("MISS    : %lu", score.judgement_counts.at(Data::Judgement::Miss));
                    ImGui::TreePop();
                }
                ImGui::TreePop();
            }
        }
        ImGui::End();
    }
}
