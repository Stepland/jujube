#include "Gameplay.hpp"

#include <chrono>
#include <optional>
#include <thread>

#include <imgui/imgui.h>
#include <imgui-sfml/imgui-SFML.h>

namespace Gameplay {
    Screen::Screen(const Data::SongDifficulty& t_song_selection) :
        song_selection(t_song_selection),
        chart(*t_song_selection.song.get_chart(t_song_selection.difficulty))
    {
        notes_left = chart.notes;
        std::variant<PreciseMusic, Silence> music;
        auto music_path = song_selection.song.full_audio_path();
        if (music_path) {
            music.emplace<PreciseMusic>(music_path->string());
        } else {
            music.emplace<Silence>(chart.get_duration_based_on_notes());
        }
        getPlayingOffset = std::visit(GetPlayingOffsetVisitor{}, music);
        getStatus = std::visit(GetStatusVisitor{}, music);
    }

    Data::Score Screen::play_chart(sf::RenderWindow& window) {
        std::thread render_thread(&Screen::render, this, std::ref(window));
        while ((not song_finished) and window.isOpen()) {
            auto playing_offset = getPlayingOffset();
            sf::Event event;
            while (window.pollEvent(event)) {
                ImGui::SFML::ProcessEvent(event);
                switch (event.type) {
                case sf::Event::KeyPressed:
                    break;
                case sf::Event::MouseButtonPressed:
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
    }
    
    void Screen::render(sf::RenderWindow& window) {
        window.setFramerateLimit(60);
        ImGui::SFML::Init(window);
        sf::Clock imguiClock;
        while ((not song_finished) and window.isOpen()) {
            ImGui::SFML::Update(window, imguiClock.restart());
            window.clear(sf::Color(7, 23, 53));
            draw_debug();
            ImGui::SFML::Render(window);
            window.display();
        }
        ImGui::SFML::Shutdown();
    }

    void Screen::handle_raw_event(const Input::Event& event, const sf::Time& playing_offset) {
        
    }

    void Screen::handle_button(const Input::Button& button, const sf::Time& playing_offset) {
        // TODO : Button Highlight

        // Is the music even playing ?
        if (getStatus() == sf::SoundSource::Playing) {
            for (auto &&note : notes_left) {
                // is the note visible ?
                if (note.timing > playing_offset + sf::seconds(16.f/30.f)) {
                    break;
                }
                // is it even the right button ?
                if (note.position != button) {
                    continue;
                }
                notes_passed.emplace(note, note.timing-playing_offset);
                notes_left.erase(note);
                break;
            }
        }
    }
}