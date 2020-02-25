#pragma once

#include <array>
#include <tuple>

#include <SFML/Graphics.hpp>

#include "../../Data/Chart.hpp"
#include "../../Data/Song.hpp"
#include "../../Toolkit/Cache.hpp"
#include "../../Toolkit/NormalizedOrigin.hpp"

namespace MusicSelect {
    class DensityGraph : public sf::Drawable, public sf::Transformable {
    public:
        explicit DensityGraph(const std::array<unsigned int, 115>& t_densities);
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        std::array<unsigned int, 115> m_densities;
        sf::VertexArray m_vertex_array;
    };

    struct SongDifficulty {
        const Data::Song& song;
        const std::string& difficulty;

        bool operator==(const SongDifficulty &other) const {
            return (
                song.folder == other.song.folder and
                difficulty == other.difficulty
            );
        }
    };

    DensityGraph compute_density_graph_from_struct(const SongDifficulty& sd);
    DensityGraph compute_density_graph_from_song_difficulty(const Data::Song& song, const std::string& difficulty);
    DensityGraph compute_density_graph_from_chart(const Data::Chart& chart, long start, long end);

    using DensityGraphCache = Toolkit::Cache<SongDifficulty, DensityGraph, &compute_density_graph_from_struct>;
}

namespace std {
    template <>
    struct hash<MusicSelect::SongDifficulty> {
        std::size_t operator()(const MusicSelect::SongDifficulty& sd) const {
            auto song_hash = std::hash<std::string>()(sd.song.folder.string());
            song_hash ^= std::hash<std::string>()(sd.difficulty) + 0x9e3779b9 + (song_hash<<6) + (song_hash>>2);
            return song_hash;
        }
    };
}

namespace Toolkit {
    template<>
    void set_origin_normalized(MusicSelect::DensityGraph& s, float x, float y);
}