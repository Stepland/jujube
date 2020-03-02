#pragma once

#include <array>
#include <vector>

#include <jbcoe/polymorphic_value.h>

#include "../../Data/Song.hpp"
#include "SharedResources.hpp"

namespace MusicSelect {

    class Panel;
    // PanelLayout restricts the ways you can create a scrollable grid of panels
    class PanelLayout : public std::vector<std::array<jbcoe::polymorphic_value<Panel>,3>> {
    public:
        explicit PanelLayout(const std::map<std::string,std::vector<jbcoe::polymorphic_value<Panel>>>& categories, SharedResources& resources);
        explicit PanelLayout(const std::vector<jbcoe::polymorphic_value<Panel>>& panels, SharedResources& resources);
        static PanelLayout red_empty_layout(SharedResources& resources);
        static PanelLayout title_sort(const Data::SongList& song_list, SharedResources& resources);
    private:
        void fill_layout(SharedResources& resources);
    };
}