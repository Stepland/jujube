#pragma once

#include "Panel.hpp"
#include "../../Data/SongList.hpp"

namespace MusicSelect {
    // The Ribbon is the moving part of the Music Select Screen
    // It can be sorted in a number of ways
    class Ribbon {
    public:
        Ribbon();
        static Ribbon title_sort(const Data::SongList& song_list);
        const auto& get_layout() {return layout;};
    private:
        std::vector<std::array<std::unique_ptr<Panel>,3>> layout;
    };
}