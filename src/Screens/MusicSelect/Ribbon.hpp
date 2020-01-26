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
        static Ribbon test2_sort();
        const auto& get_layout() {return layout;};
        const std::shared_ptr<MusicSelect::Panel>& at(unsigned int button_index) const;
        void move_right();
        void move_left();
    private:
        static Ribbon layout_from_map(const std::map<std::string,std::list<std::shared_ptr<Panel>>>& categories);
        std::vector<std::array<std::shared_ptr<Panel>,3>> layout;
        unsigned int position = 0;
    };
}