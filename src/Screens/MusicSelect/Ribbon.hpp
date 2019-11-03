#pragma once

#include "Panel.hpp"
#include "../../Data/SongList.hpp"

namespace MusicSelect {
    // The Ribbon is the moving part of the Music Select Screen
    // It can be sorted in a number of ways
    class Ribbon {
    public:
        Ribbon() = default;
        static Ribbon title_sort(const Data::SongList& song_list);
        static Ribbon test_sort();
        const auto& get_layout() {return layout;};
        const std::unique_ptr<MusicSelect::Panel>& at(unsigned int button_index) const;
    private:
        std::vector<std::array<std::unique_ptr<Panel>,3>> layout;
        unsigned int position = 0;
    };
}