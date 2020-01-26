#include "Ribbon.hpp"

#include <cstdlib>
#include <map>
#include <vector>

#include "Panel.hpp"
#include "../../Toolkit/QuickRNG.hpp"

MusicSelect::Ribbon MusicSelect::Ribbon::title_sort(const Data::SongList& song_list) {
    std::map<char,std::vector<Data::Song>> categories;
    for (const auto& song : song_list.songs) {
        if (song.title.size() > 0) {
            char letter = song.title[0];
            if ('A' <= letter and letter <= 'Z') {
                categories[letter].push_back(song);
            } else if ('a' <= letter and letter <= 'z') {
                categories['A' + (letter - 'a')].push_back(song);
            } else {
                categories['?'].push_back(song);
            }
        } else {
            categories['?'].push_back(song);
        }
    }
    Ribbon ribbon;
    for (auto& [letter, songs] : categories) {
        std::vector<std::shared_ptr<Panel>> panels;
        panels.emplace_back(
            std::make_shared<CategoryPanel>(
                std::string(1, letter)
            )
        );
        std::sort(songs.begin(), songs.end(), Data::Song::sort_by_title);
        for (const auto& song : songs) {
            panels.push_back(std::make_shared<SongPanel>(song));
        }
        while (panels.size() % 3 != 0) {
            panels.push_back(std::make_shared<EmptyPanel>());
        }
        for (size_t i = 0; i < panels.size(); i += 3) {
            ribbon.layout.emplace_back();
            for (size_t j = 0; j < 3; j++) {
                ribbon.layout.back()[j] = std::move(panels[i+j]);
            }
        }
    }
    return ribbon;
}

MusicSelect::Ribbon MusicSelect::Ribbon::test_sort() {
    Ribbon ribbon;
    ribbon.layout.push_back(
        {
            std::make_shared<EmptyPanel>(),
            std::make_shared<CategoryPanel>("A"),
            std::make_shared<CategoryPanel>("truc")
        }
    );
    for (size_t i = 0; i < 3; i++) {
        ribbon.layout.push_back(
            {
                std::make_shared<EmptyPanel>(),
                std::make_shared<EmptyPanel>(),
                std::make_shared<EmptyPanel>()
            }
        );
    }
    return ribbon;
}

MusicSelect::Ribbon MusicSelect::Ribbon::test2_sort() {
    std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::map<std::string, std::list<std::shared_ptr<Panel>>> categories;
    Toolkit::UniformIntRNG category_size_generator{1,10};
    Toolkit::UniformIntRNG panel_hue_generator{0,255};
    for (auto &&letter : alphabet) {
        auto category_size = category_size_generator.generate();
        for (int i = 0; i < category_size; i++) {
            categories[std::string(1, letter)].push_back(
                std::make_shared<MusicSelect::ColorPanel>(
                    sf::Color(
                        panel_hue_generator.generate(),
                        panel_hue_generator.generate(),
                        panel_hue_generator.generate()
                    )
                )
            );
        }
    }
    return Ribbon::layout_from_map(categories);
}

const std::shared_ptr<MusicSelect::Panel>& MusicSelect::Ribbon::at(unsigned int button_index) const {
    return (
        layout
        .at((position + (button_index % 4)) % layout.size())
        .at(button_index / 4)
    );
}

MusicSelect::Ribbon MusicSelect::Ribbon::layout_from_map(const std::map<std::string, std::list<std::shared_ptr<MusicSelect::Panel>>>& categories) {
    Ribbon ribbon;
    for (auto &&[category, panels] : categories) {
        if (not panels.empty()) {
            std::vector<std::shared_ptr<Panel>> current_column;
            current_column.push_back(std::make_shared<CategoryPanel>(category));
            for (auto &&panel : panels) {
                if (current_column.size() == 3) {
                    ribbon.layout.push_back({current_column[0], current_column[1], current_column[2]});
                    current_column.clear();
                } else {
                    current_column.push_back(std::move(panel));
                }
            }
            if (not current_column.empty()) {
                while (current_column.size() < 3) {
                    current_column.push_back(std::make_shared<EmptyPanel>());
                }
                ribbon.layout.push_back({current_column[0], current_column[1], current_column[2]});
            }
        }
    }
    return ribbon;
}

void MusicSelect::Ribbon::move_right() {
    position = (position + 1) % layout.size();
}

void MusicSelect::Ribbon::move_left() {
    if (position == 0) {
        position = layout.size() - 1;
    } else {
        position--;
    }
}
