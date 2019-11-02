#include "Ribbon.hpp"

#include <map>
#include <vector>

#include "Panel.hpp"

MusicSelect::Ribbon MusicSelect::Ribbon::title_sort(const Data::SongList& song_list) {
    std::map<char,std::vector<Data::Song>> categories;
    for (const auto& song : song_list.songs) {
        if (song.title.size > 0) {
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
    for (const auto& [letter, songs] : categories) {
        std::vector<std::unique_ptr<Panel>> panels = {std::make_unique<CategoryPanel>(letter)};
        std::sort(songs.begin(), songs.end(), Data::Song::sort_by_title);
        for (const auto& song : songs) {
            panels.push_back(std::make_unique<SongPanel>(song));
        }
        while (panels.size % 3 != 0) {
            panels.push_back(std::make_unique<EmptyPanel>());
        }
        for (size_t i = 0; i < panels.size; i += 3) {
            std::array<std::unique_ptr<Panel>,3> column = {
                std::move(panels[i]),
                std::move(panels[i+1]),
                std::move(panels[i+2])
            };
            ribbon.layout.push_back(column);
        }
    }
    return ribbon;
}