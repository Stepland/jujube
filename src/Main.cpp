#include <iostream>

#include <SFML/Graphics.hpp>
#include <whereami/whereami++.hpp>

#include "Data/Song.hpp"
#include "Data/Preferences.hpp"
#include "Resources/Marker.hpp"
#include "Resources/SharedResources.hpp"
// #include "Data/Chart.hpp"
// #include "Data/Score.hpp"

#include "Screens/MusicSelect/Resources.hpp"
#include "Screens/MusicSelect/MusicSelect.hpp"
// #include "Screens/Gameplay.hpp"
// #include "Screens/Result.hpp"

int main(int, char const **) {

    // Load prefs, music, markers
    const std::string jujube_path = whereami::executable_dir();
    Data::Preferences preferences{jujube_path};
    Data::SongList song_list{jujube_path};
    Resources::SharedResources shared_resources{preferences};
    MusicSelect::ScreenResources music_select_resources{shared_resources};
    if (shared_resources.markers.find(preferences.options.marker) == shared_resources.markers.end()) {
        preferences.options.marker = shared_resources.markers.begin()->first;
    }
    MusicSelect::Screen music_select{song_list, music_select_resources};    
    
    // Create the window
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window{
        sf::VideoMode(
            preferences.screen.width,
            preferences.screen.height
        ),
        "jujube",
        preferences.screen.fullscreen ? sf::Style::Fullscreen : sf::Style::Default,
        settings
    };
    auto chart = music_select.select_chart(window);
    if (chart) {
        std::cout << "Selected Chart : " << chart->song.title << " [" << chart->difficulty << "]" << std::endl;
    } else {
        std::cout << "Exited MusicSelect::Screen without selecting a chart" << std::endl;
    }
    /*
    while (true) {
        
        Chart& selected_chart = music_select.select_chart(window);

        Screen::Gameplay gameplay(selected_chart);
        Score score = gameplay.play_chart(window);
        
        Screen::Result result_screen(score);
        result_screen.display(window);
    
    }
    */
    
    return 0;

}
