#pragma once

#include <array>
#include <tuple>

#include "../../Data/Chart.hpp"
#include "../../Data/Song.hpp"
#include "../../Toolkit/Cache.hpp"

namespace Data {
    using DensityGraph = std::array<unsigned int, 115>;

    struct SongDifficulty {
        const Song& song;
        const std::string& difficulty;

        bool operator==(const SongDifficulty &other) const {
            return (
                song.folder == other.song.folder and
                difficulty == other.difficulty
            );
        }
    };

    DensityGraph compute_density_graph_1(const SongDifficulty& sd);
    DensityGraph compute_density_graph_2(const Song& song, const std::string& difficulty);
    DensityGraph compute_density_graph_3(const Chart& chart, long start, long end);

    using DensityGraphCache = Toolkit::Cache<SongDifficulty, DensityGraph, &compute_density_graph_1>;
}

namespace std {
    template <>
    struct hash<Data::SongDifficulty> {
        std::size_t operator()(const Data::SongDifficulty& sd) const {
            auto song_hash = std::hash<std::string>()(sd.song.folder.string());
            song_hash ^= std::hash<std::string>()(sd.difficulty) + 0x9e3779b9 + (song_hash<<6) + (song_hash>>2);
            return song_hash;
        }
    };
}