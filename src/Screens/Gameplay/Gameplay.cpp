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
#include "../../Toolkit/NormalizedOrigin.hpp"
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
            // Draw Combo
            auto combo_now = combo.load();
            if (combo_now >= 4) {
                sf::Text combo_text;
                combo_text.setFont(shared.fallback_font.black);
                combo_text.setFillColor(sf::Color(23, 78, 181));
                combo_text.setString(std::to_string(combo_now));
                combo_text.setCharacterSize(static_cast<unsigned int>(1.5*get_panel_step()));
                Toolkit::set_local_origin_normalized(combo_text, 0.5f, 0.5f);
                combo_text.setPosition(
                    get_ribbon_x()+get_ribbon_size()*0.5f,
                    get_ribbon_y()+get_ribbon_size()*0.5f
                );
                window.draw(combo_text);
            }

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
                    auto rect = sprite->getLocalBounds();
                    sprite->setScale(get_panel_size()/rect.width, get_panel_size()/rect.height);
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
            draw_debug();
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
                ImGui::Text("Raw : %d", score.get_score());
                ImGui::Text("Final : %d", score.get_final_score());
                if (ImGui::TreeNode("Counts")) {
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
