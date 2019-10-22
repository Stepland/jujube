#include <SFML/Graphics.hpp>

#include "Model/Chart.hpp"
#include "Model/MusicList.hpp"
#include "Model/Score.hpp"

#include "Screens/MusicSelect.hpp"
#include "Screens/Gameplay.hpp"
#include "Screens/Result.hpp"

int main(int argc, char const *argv[]) {
    sf::RenderWindow window(sf::VideoMode(800,600), "jujube");
    window.setVerticalSyncEnabled(true);
    Screen::MusicSelect music_select;
    while (true) {
        Chart& selected_chart = music_select.display(window);
        Screen::Gameplay gameplay(selected_chart);
        Score score = gameplay.display(window);
        Screen::Result result(score);
        result.display(window);
    }
    return 0;
}
