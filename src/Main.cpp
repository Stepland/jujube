#include <iostream>
#include <sstream>

#include <SFML/Graphics.hpp>
#include <cereal/archives/json.hpp>

#include "Data/SongList.hpp"
#include "Data/Preferences.hpp"
// #include "Data/Chart.hpp"
// #include "Data/Score.hpp"

#include "Screens/MusicSelect/MusicSelect.hpp"
// #include "Screens/Gameplay.hpp"
// #include "Screens/Result.hpp"

int main(int argc, char const *argv[]) {

    std::cout << ss.str() << std::endl;
    return 0;
    
    sf::RenderWindow window;
    
    Data::SongList song_list;
    Data::Preferences preferences;
    MusicSelect::Screen music_select{song_list};
    
    music_select.select_chart(window);
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
