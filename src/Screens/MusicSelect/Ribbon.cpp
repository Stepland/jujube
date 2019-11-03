#include "Ribbon.hpp"

#include <map>
#include <vector>

#include "Panel.hpp"

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
        std::vector<std::unique_ptr<Panel>> panels;
        panels.emplace_back(
            std::make_unique<CategoryPanel>(
                std::string(1, letter)
            )
        );
        std::sort(songs.begin(), songs.end(), Data::Song::sort_by_title);
        for (const auto& song : songs) {
            panels.push_back(std::make_unique<SongPanel>(song));
        }
        while (panels.size() % 3 != 0) {
            panels.push_back(std::make_unique<EmptyPanel>());
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
            std::make_unique<EmptyPanel>(),
            std::make_unique<CategoryPanel>("A"),
            std::make_unique<CategoryPanel>("truc")
        }
    );
    for (size_t i = 0; i < 3; i++) {
        ribbon.layout.push_back(
            {
                std::make_unique<EmptyPanel>(),
                std::make_unique<EmptyPanel>(),
                std::make_unique<EmptyPanel>()
            }
        );
    }
    return ribbon;
}

const std::unique_ptr<MusicSelect::Panel>& MusicSelect::Ribbon::at(unsigned int button_index) const {
    return (
        layout
        .at((position + (button_index % 4)) % layout.size())
        .at(button_index / 4)
    );
}
