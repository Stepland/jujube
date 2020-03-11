#include "Gameplay.hpp"

#include <imgui/imgui.h>
#include <imgui-sfml/imgui-SFML.h>

namespace Gameplay {
    Screen::Screen(const Data::SongDifficulty& t_song_selection) :
        song_selection(t_song_selection),
        chart(*t_song_selection.song.get_chart(t_song_selection.difficulty))
    {
        
    }

    Data::Score Screen::play_chart(sf::RenderWindow& window) {
        window.setFramerateLimit(60);
        ImGui::SFML::Init(window);
        sf::Clock imguiClock;
        sf::Music song;
        if (song_selection.song.full_audio_path())
        while ((not song_finished) and window.isOpen()) {
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
                    // resources.preferences.screen.height = event.size.height;
                    // resources.preferences.screen.width = event.size.width;
                    break;
                default:
                    break;
                }
            }
            ImGui::SFML::Update(window, imguiClock.restart());
            window.clear(sf::Color(7, 23, 53));
            draw_debug();
            ImGui::SFML::Render(window);
            window.display();
        }
        ImGui::SFML::Shutdown();
    }
}
