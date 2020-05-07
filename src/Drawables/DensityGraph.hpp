#pragma once

#include <array>
#include <tuple>

#include <SFML/Graphics.hpp>

#include "../Data/Chart.hpp"
#include "../Data/Song.hpp"
#include "../Toolkit/Cache.hpp"
#include "../Toolkit/SFMLHelpers.hpp"

namespace Drawables {
    class DensityGraph : public sf::Drawable, public sf::Transformable {
    public:
        explicit DensityGraph(const std::array<unsigned int, 115>& t_densities);
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        std::array<unsigned int, 115> m_densities;
        sf::VertexArray m_vertex_array;
    };

    DensityGraph compute_density_graph_from_struct(const Data::SongDifficulty& sd);
    DensityGraph compute_density_graph_from_song_difficulty(const Data::Song& song, const std::string& difficulty);
    DensityGraph compute_density_graph_from_chart(const Data::Chart& chart, sf::Time start, sf::Time end);

    using DensityGraphCache = Toolkit::Cache<Data::SongDifficulty, DensityGraph, &compute_density_graph_from_struct>;
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

namespace Toolkit {
    template<>
    void set_origin_normalized(Drawables::DensityGraph& s, float x, float y);
}