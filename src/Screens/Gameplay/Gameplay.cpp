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
#include "../../Toolkit/AffineTransform.hpp"
#include "../../Toolkit/SFMLHelpers.hpp"
#include "PreciseMusic.hpp"
#include "Silence.hpp"

namespace Gameplay {

    Toolkit::AffineTransform<float> get_music_time_to_progression_transform(const Data::SongDifficulty& song_selection) {
        auto time_bounds = song_selection.get_time_bounds();
        return Toolkit::AffineTransform<float>{
            time_bounds.start.asSeconds(),
            time_bounds.end.asSeconds(),
            0.f,
            1.f
        };
    }

    Screen::Screen(const Data::SongDifficulty& t_song_selection, ScreenResources& t_resources) :
        HoldsResources(t_resources),
        song_selection(t_song_selection),
        chart(*t_song_selection.song.get_chart(t_song_selection.difficulty)),
        marker(t_resources.shared.get_selected_marker()),
        ln_marker(t_resources.shared.get_selected_ln_marker()),
        graded_density_graph(t_resources.shared.density_graphs.blocking_get(t_song_selection), t_song_selection),
        music_time_to_progression(get_music_time_to_progression_transform(t_song_selection)),
        score(t_song_selection.song.get_chart(t_song_selection.difficulty)->notes)
    {
        for (auto&& note : chart.notes) {
            notes.emplace_back(Data::GradedNote{note});
        }
        auto music_path = song_selection.song.full_audio_path();
        if (music_path) {
            music = std::make_unique<PreciseMusic>(music_path->string());
        } else {
            music = std::make_unique<Silence>(chart.get_last_event_timing());
        }
        ln_tail_layer.setSmooth(true);
        marker_layer.setSmooth(true);
    }

    DetailedScore Screen::play_chart(sf::RenderWindow& window) {
        window.setKeyRepeatEnabled(false);
        window.setActive(false);
        std::thread render_thread(&Screen::render, this, std::ref(window));
        while ((not song_finished) and window.isOpen()) {
            auto music_time = music->getPlayingOffset() - preferences.options.audio_offset;
            sf::Event event;
            while (window.pollEvent(event)) {
                ImGui::SFML::ProcessEvent(event);
                events_queue.push({music_time, event});
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        if (render_thread.joinable()) {
            render_thread.join();
        }
        return {graded_density_graph, score};
    }
    
    void Screen::render(sf::RenderWindow& window) {
        window.setActive(true);
        auto window_size = window.getSize();
        ln_tail_layer.create(window_size.x, window_size.y);
        marker_layer.create(window_size.x, window_size.y);
        sf::Clock imguiClock;
        music->play();
        while ((not song_finished) and window.isOpen()) {
            song_finished = music->getStatus() == sf::Music::Stopped;
            ImGui::SFML::Update(window, imguiClock.restart());
            auto music_time = music->getPlayingOffset() - preferences.options.audio_offset;
            update_visible_notes(music_time);
            graded_density_graph.update(music_time);

            // Event Handling
            while (auto timed_event = events_queue.pop()) {
                switch (timed_event->event.type) {
                case sf::Event::KeyPressed:
                    handle_raw_input_event({timed_event->event.key.code, Input::EventType::Pressed}, timed_event->time);
                    break;
                case sf::Event::KeyReleased:
                    handle_raw_input_event({timed_event->event.key.code, Input::EventType::Released}, timed_event->time);
                    break;
                case sf::Event::JoystickButtonPressed:
                    handle_raw_input_event({timed_event->event.joystickButton, Input::EventType::Pressed}, timed_event->time);
                    break;
                case sf::Event::JoystickButtonReleased:
                    handle_raw_input_event({timed_event->event.joystickButton, Input::EventType::Released}, timed_event->time);
                    break;
                case sf::Event::MouseButtonPressed:
                    handle_mouse_click(timed_event->event.mouseButton, timed_event->time);
                    break;
                case sf::Event::MouseButtonReleased:
                    handle_mouse_release(timed_event->event.mouseButton, timed_event->time);
                    break;
                case sf::Event::MouseMoved:
                    handle_mouse_move(timed_event->event.mouseMove, timed_event->time);
                    break;
                case sf::Event::TouchBegan:
                    handle_touch_began(timed_event->event.touch, timed_event->time);
                    break;
                case sf::Event::TouchMoved:
                    handle_touch_moved(timed_event->event.touch, timed_event->time);
                    break;
                case sf::Event::TouchEnded:
                    handle_touch_ended(timed_event->event.touch, timed_event->time);
                    break;
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::Resized:
                    // update the view to the new size of the window
                    window.setView(
                        sf::View(
                            {
                                0,
                                0,
                                static_cast<float>(timed_event->event.size.width),
                                static_cast<float>(timed_event->event.size.height)
                            }
                        )
                    );
                    ln_tail_layer.create(timed_event->event.size.width, timed_event->event.size.height);
                    marker_layer.create(timed_event->event.size.width, timed_event->event.size.height);
                    preferences.screen.video_mode.height = timed_event->event.size.height;
                    preferences.screen.video_mode.width = timed_event->event.size.width;
                    shared.button_highlight.setPosition(get_ribbon_x(), get_ribbon_y());
                    break;
                default:
                    break;
                }
            }
            window.clear(sf::Color(7, 23, 53));
            ln_tail_layer.clear(sf::Color::Transparent);
            marker_layer.clear(sf::Color::Transparent);

            // Don't display shutter for now : it's fucking ugly
            // TODO: make fallback shutter not sinfully ugly
            /*
            shutter.update(score.get_shutter());
            shutter.setScale(get_ribbon_size()/1080.f, get_ribbon_size()/1080.f);
            shutter.setPosition(get_ribbon_x(), get_ribbon_y());
            window.draw(shutter);
            */

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

            // Cursor on the density graph
            Toolkit::set_local_origin_normalized(cursor, 1.f, 0.f);
            auto bounds = graded_density_graph.getGlobalBounds();
            cursor.setScale(bounds.height, bounds.height);
            cursor.setPosition(
                bounds.left+music_time_to_progression.transform(music_time.asSeconds())*bounds.width,
                bounds.top
            );
            window.draw(cursor);


            // Draw Combo
            if (combo >= 4) {
                sf::Text combo_text;
                combo_text.setFont(shared.fallback_font.black);
                combo_text.setFillColor(sf::Color(18, 59, 135));
                combo_text.setString(std::to_string(combo));
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

            // Draw Notes, longs first then normal ones
            for (auto &&note_ref : visible_notes) {
                const auto& note = note_ref.get();
                if (note.duration == sf::Time::Zero) {
                    draw_tap_note(note, music_time);
                } else {
                    draw_long_note(note, music_time);
                }
            }
            ln_tail_layer.display();
            marker_layer.display();
            window.draw(sf::Sprite{ln_tail_layer.getTexture()});
            window.draw(sf::Sprite{marker_layer.getTexture()});

            shared.button_highlight.update();
            window.draw(shared.button_highlight);
            if (display_black_bars) {
                window.draw(shared.black_frame);
            }
            if (debug) {
                draw_debug();
            }
            ImGui::SFML::Render(window);
            window.display();
        }
    }

    void Screen::draw_tap_note(const Data::GradedNote& note, const sf::Time& music_time) {
        std::optional<sf::Sprite> sprite;
        if (note.tap_judgement) {
            sprite = marker.get_sprite(
                Data::judgement_to_animation(note.tap_judgement->judgement),
                music_time-note.timing-note.tap_judgement->delta
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
            marker_layer.draw(*sprite);
        }
    }

    void Screen::draw_long_note(const Data::GradedNote& note, const sf::Time& music_time) {
        if (not note.long_release) {
            // long note did not finish yet
            // No need to display the background and tail if the first tap judgement at the beginning broke combo
            if (
                (not note.tap_judgement) or
                (note.tap_judgement and not Data::judgement_breaks_combo(note.tap_judgement->judgement))
            ) {
                auto note_offset = music_time-note.timing;
                auto note_coords = Input::button_to_coords(note.position);
                sf::Vector2f note_position{
                    get_ribbon_x() + (get_panel_step()*note_coords.x) + get_panel_size()/2.f,
                    get_ribbon_y() + (get_panel_step()*note_coords.y) + get_panel_size()/2.f
                };
                float tail_angle = 90.f * note.get_tail_angle();
                float scale = get_panel_size() / ln_marker.size;
                Toolkit::AffineTransform<float> delta_to_normalized_tip_distance(
                    0.f,
                    note.duration.asSeconds(),
                    static_cast<float>(note.get_tail_length()),
                    0.f
                );
                float normalized_tail_length = 
                    delta_to_normalized_tip_distance.clampedTransform(note_offset.asSeconds());
                float tail_length;
                auto frame = static_cast<int>(std::floor(note_offset.asSeconds()*ln_marker.fps));
                if (frame < static_cast<int>(ln_marker.tip_enter_cycle.count)) {
                    // During the tip animation : tail goes from tip to note edge
                    tail_length = std::max(
                        0.f,
                        ln_marker.size * (normalized_tail_length*(get_panel_step()/get_panel_size()) - 1.0f)
                    );
                } else {
                    // After the tip animation : tail goes from half of the triangle base to note edge
                    tail_length = std::max(
                        0.f,
                        ln_marker.size * (normalized_tail_length*(get_panel_step()/get_panel_size()) - 0.5f)
                    );
                }
                if (auto tail_sprite = ln_marker.get_tail_sprite(note_offset)) {
                    Toolkit::set_origin_normalized(*tail_sprite, 0.5f, -0.5f);
                    auto rect = tail_sprite->getTextureRect();
                    rect.height = static_cast<int>(tail_length);
                    tail_sprite->setTextureRect(rect);
                    tail_sprite->setPosition(note_position);
                    tail_sprite->rotate(tail_angle+180.f);
                    tail_sprite->setScale(scale, scale);
                    ln_tail_layer.draw(*tail_sprite);
                }
                if (auto tip_sprite = ln_marker.get_tip_sprite(note_offset)) {
                    Toolkit::set_origin_normalized(
                        *tip_sprite,
                        0.5f,
                        0.5f + (get_panel_step()/get_panel_size())*normalized_tail_length
                    );
                    tip_sprite->setPosition(note_position);
                    tip_sprite->setRotation(tail_angle);
                    tip_sprite->setScale(scale, scale);
                    ln_tail_layer.draw(*tip_sprite);
                }
                if (auto background_sprite = ln_marker.get_background_sprite(note_offset)) {
                    Toolkit::set_origin_normalized(*background_sprite, 0.5f, 0.5f);
                    background_sprite->setPosition(note_position);
                    background_sprite->setRotation(tail_angle);
                    background_sprite->setScale(scale, scale);
                    ln_tail_layer.draw(*background_sprite);
                }
                if (auto outline_sprite = ln_marker.get_outline_sprite(note_offset)) {
                    Toolkit::set_origin_normalized(*outline_sprite, 0.5f, 0.5f);
                    outline_sprite->setPosition(note_position);
                    outline_sprite->setRotation(tail_angle);
                    outline_sprite->setScale(scale, scale);
                    ln_tail_layer.draw(*outline_sprite);                  
                }
                if (auto highlight_sprite = ln_marker.get_highlight_sprite(note_offset)) {
                    Toolkit::set_origin_normalized(*highlight_sprite, 0.5f, 0.5f);
                    highlight_sprite->setPosition(note_position);
                    highlight_sprite->setRotation(tail_angle);
                    highlight_sprite->setScale(scale, scale);
                    ln_tail_layer.draw(*highlight_sprite);
                }
            }
            draw_tap_note(note, music_time);
        } else {
            // long note ended, only draw the ending marker
            std::optional<sf::Sprite> sprite;
            sprite = marker.get_sprite(
                Data::judgement_to_animation(note.long_release->judgement),
                music_time-note.timing-note.duration-note.long_release->delta
            );
            if (sprite) {
                Toolkit::set_size_from_local_bounds(*sprite, get_panel_size(), get_panel_size());
                auto pos = Input::button_to_coords(note.position);
                sprite->setPosition(
                    get_ribbon_x()+get_panel_step()*pos.x,
                    get_ribbon_y()+get_panel_step()*pos.y
                );
                marker_layer.draw(*sprite);
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

    void Screen::handle_mouse_click(const sf::Event::MouseButtonEvent& mouse_event, const sf::Time& music_time) {
        if (mouse_event.button != sf::Mouse::Button::Left) {
            return;
        }
        last_mouse_clicked_button = button_from_position({mouse_event.x, mouse_event.y});
        if (last_mouse_clicked_button) {
            handle_button_event({*last_mouse_clicked_button, Input::EventType::Pressed}, music_time);
        }
    }

    void Screen::handle_mouse_move(const sf::Event::MouseMoveEvent& mouse_move_event, const sf::Time& music_time) {
        if (not sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            return;
        }
        auto current_button = button_from_position({mouse_move_event.x, mouse_move_event.y});
        if (current_button != last_mouse_clicked_button) {
            if (last_mouse_clicked_button) {
                handle_button_event({*last_mouse_clicked_button, Input::EventType::Released}, music_time);
            }
            if (current_button) {
                last_mouse_clicked_button = current_button;
                handle_button_event({*last_mouse_clicked_button, Input::EventType::Pressed}, music_time);
            }
        }
    }

    void Screen::handle_mouse_release(const sf::Event::MouseButtonEvent& mouse_event, const sf::Time& music_time) {
        if (mouse_event.button != sf::Mouse::Button::Left) {
            return;
        }
        if (last_mouse_clicked_button) {
            handle_button_event({*last_mouse_clicked_button, Input::EventType::Released}, music_time);
            last_mouse_clicked_button.reset();
        }
    }

    void Screen::handle_touch_began(const sf::Event::TouchEvent& touch_event, const sf::Time& music_time) {
        auto& last_button = last_touched_button[touch_event.finger];
        last_button = button_from_position({touch_event.x, touch_event.y});
        if (last_button) {
            handle_button_event({*last_button, Input::EventType::Pressed}, music_time);
        }
    }

    void Screen::handle_touch_moved(const sf::Event::TouchEvent& touch_event, const sf::Time& music_time) {
        auto& last_button = last_touched_button[touch_event.finger];
        auto current_button = button_from_position({touch_event.x, touch_event.y});
        if (current_button != last_button) {
            if (last_button) {
                handle_button_event({*last_button, Input::EventType::Released}, music_time);
            }
            if (current_button) {
                last_button = current_button;
                handle_button_event({*last_button, Input::EventType::Pressed}, music_time);
            }
        }
    }

    void Screen::handle_touch_ended(const sf::Event::TouchEvent& touch_event, const sf::Time& music_time) {
        auto& last_button = last_touched_button[touch_event.finger];
        if (last_button) {
            handle_button_event({*last_button, Input::EventType::Released}, music_time);
            last_button.reset();
        }
    }

    void Screen::handle_raw_input_event(const Input::RawEvent& raw_event, const sf::Time& music_time) {
        auto button = preferences.key_mapping.key_to_button(raw_event.key);
        if (button) {
            handle_button_event({*button, raw_event.type}, music_time);
        } else if (raw_event.type == Input::EventType::Pressed) {
            if (auto key = std::get_if<sf::Keyboard::Key>(&raw_event.key)) {
                switch (*key) {
                case sf::Keyboard::F12:
                    debug = not debug;
                    graded_density_graph.debug = not graded_density_graph.debug;
                    break;
                default:
                    break;
                }
            }
        }
    }

    void Screen::handle_button_event(const Input::ButtonEvent& button_event, const sf::Time& music_time) {
        shared.button_highlight.handle_button_event(button_event);
        // Is the music even playing ?
        if (music->getStatus() == sf::SoundSource::Playing) {
            switch (button_event.type) {
            case Input::EventType::Pressed:
                handle_button_press(button_event.button, music_time);
                break;
            case Input::EventType::Released:
                handle_button_release(button_event.button, music_time);
                break;
            }
        }
    }

    void Screen::handle_button_press(const Input::Button& button, const sf::Time& music_time) {
        for (auto&& note_ref : visible_notes) {
            auto& note = note_ref.get();
            // is it even the right button ?
            if (note.position != button) {
                continue;
            }
            // has it already been graded ?
            if (note.tap_judgement) {
                continue;
            }
            note = Data::GradedNote{note, music_time-note.timing};
            auto& judgement = note.tap_judgement->judgement;
            score.update(judgement);
            // Late/early
            if((music_time-note.timing > sf::milliseconds(0)) && note.tap_judgement->judgement != Data::Judgement::Perfect)
                score.update(Data::Judgement::Late);
            if((music_time-note.timing < sf::milliseconds(0)) && note.tap_judgement->judgement != Data::Judgement::Perfect)
                score.update(Data::Judgement::Early);
            graded_density_graph.update_grades(judgement, note.timing);
            if (Data::judgement_breaks_combo(judgement)) {
                // If we've broken combo at the begining of a long we also missed the end
                if (note.duration > sf::Time::Zero) {
                    note.long_release = Data::TimedJudgement{sf::Time::Zero, Data::Judgement::Miss};
                    score.update(Data::Judgement::Miss);
                    graded_density_graph.update_grades(Data::Judgement::Miss, note.timing+note.duration);
                }
                combo = 0;
            } else {
                combo++;
            }
            return;
        }
    }
    // TODO: FIX RELEASE TIMING CODE
    void Screen::handle_button_release(const Input::Button& button, const sf::Time& music_time) {
        for (auto&& note_ref : visible_notes) {
            auto& note = note_ref.get();
            // is it a long note ?
            if (note.duration == sf::Time::Zero) {
                continue;
            }
            // is it even the right button ?
            if (note.position != button) {
                continue;
            }
            // has it been correctly hit ?
            if (not note.tap_judgement) {
                continue;
            }
            // has it already been graded for release ?
            if (note.long_release) {
                continue;
            }
            auto timed_judgement = Data::TimedJudgement{music_time-note.timing-note.duration};
            note.long_release = timed_judgement;
            score.update(timed_judgement.judgement);
            graded_density_graph.update_grades(timed_judgement.judgement, note.timing+note.duration);
            if (Data::judgement_breaks_combo(timed_judgement.judgement)) {
                combo = 0;
            } else {
                combo++;
            }
            return;
        }
    }

    void Screen::miss_old_notes(const sf::Time& music_time) {
        for (auto &&note_ref : visible_notes) {
            auto& note = note_ref.get();
            if (
                (not note.tap_judgement.has_value())
                and (note.timing < music_time - sf::seconds(16.f/30.f))
            ) {
                auto timed_judgement = Data::TimedJudgement{sf::Time::Zero, Data::Judgement::Miss};
                note.tap_judgement = timed_judgement;
                score.update(Data::Judgement::Miss);
                graded_density_graph.update_grades(Data::Judgement::Miss, note.timing);
                if (note.duration > sf::Time::Zero) {
                    note.long_release = timed_judgement;
                    score.update(Data::Judgement::Miss);
                    graded_density_graph.update_grades(Data::Judgement::Miss, note.timing+note.duration);
                }
                combo = 0;
            }
        }
    }

    void Screen::release_finished_longs(const sf::Time& music_time) {
        for (auto &&note_ref : visible_notes) {
            auto& note = note_ref.get();
            if (
                (note.duration > sf::Time::Zero)
                and (note.timing + note.duration < music_time)
                and note.tap_judgement.has_value()
                and not Data::judgement_breaks_combo(note.tap_judgement->judgement)
                and (not note.long_release.has_value())
            ) {
                auto timed_judgement = Data::TimedJudgement{sf::Time::Zero, Data::Judgement::Perfect};
                note.long_release = timed_judgement;
                score.update(timed_judgement.judgement);
                graded_density_graph.update_grades(timed_judgement.judgement, note.timing + note.duration);
                combo++;
            }
        }
    }

    void Screen::update_visible_notes(const sf::Time& music_time) {
        // Mark old notes as missed and finished longs as released
        miss_old_notes(music_time);
        release_finished_longs(music_time);
        
        // Remove old notes
        visible_notes.erase(
            std::remove_if(visible_notes.begin(), visible_notes.end(),
                [music_time](Data::GradedNote& note) -> bool {
                    return note.timing + note.duration < music_time - sf::seconds(16.f/30.f);
                }
            ),
            visible_notes.end()
        );
        
        // Add new notes
        Data::GradedNote lower_note, upper_note;
        if (visible_notes.empty()) {
            lower_note.position = Input::Button::B16;
            lower_note.timing = music_time - sf::seconds(16.f/30.f);
        } else {
            lower_note = *visible_notes.rbegin();
        }
        upper_note.position = Input::Button::B1;
        upper_note.timing = music_time + sf::seconds(16.f/30.f);
        auto new_notes_begin = std::upper_bound(notes.begin(), notes.end(), lower_note);
        auto new_notes_end = std::upper_bound(new_notes_begin, notes.end(), upper_note);
        std::for_each(new_notes_begin, new_notes_end,
            [this](Data::GradedNote& note){
                this->visible_notes.emplace_back(note);
            }
        );
    }

    void Screen::draw_debug() {
        if (ImGui::Begin("Gameplay Debug")) {
            if (ImGui::CollapsingHeader("Metrics")) {
                ImGui::Text("Combo : %zu", combo);
                if (ImGui::TreeNode("Score")) {
                    ImGui::Text("Raw           : %d", score.get_score());
                    ImGui::Text("Final         : %d", score.get_final_score());
                    ImGui::Text("Shutter value : %d", score.shutter);
                    if (ImGui::TreeNode("Judgement Counts")) {
                        ImGui::Text("PERFECT : %zu", score.judgement_counts.at(Data::Judgement::Perfect));
                        ImGui::Text("GREAT   : %zu", score.judgement_counts.at(Data::Judgement::Great));
                        ImGui::Text("GOOD    : %zu", score.judgement_counts.at(Data::Judgement::Good));
                        ImGui::Text("POOR    : %zu", score.judgement_counts.at(Data::Judgement::Poor));
                        ImGui::Text("MISS    : %zu", score.judgement_counts.at(Data::Judgement::Miss));
                        ImGui::TreePop();
                    }
                    ImGui::TreePop();
                }
            }
            if (ImGui::CollapsingHeader("Graphics")) {
                ImGui::Columns(2, "graphics toogles");
                ImGui::TextDisabled("Key"); ImGui::NextColumn();
                ImGui::TextDisabled("Value"); ImGui::NextColumn();
                ImGui::Separator();
                ImGui::TextUnformatted("black frame"); ImGui::NextColumn();
                if (ImGui::Button(display_black_bars?"shown":"hidden")) {
                    display_black_bars = not display_black_bars;
                }
            }
        }
        ImGui::End();
    }
}
