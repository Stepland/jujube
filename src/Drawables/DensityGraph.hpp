#pragma once

#include <array>
#include <tuple>

#include <SFML/Graphics.hpp>

#include "../Data/Chart.hpp"
#include "../Data/Song.hpp"
#include "../Data/TimeBounds.hpp"
#include "../Toolkit/Cache.hpp"
#include "../Toolkit/SFMLHelpers.hpp"

namespace Drawables {
    class DensityGraph : public sf::Drawable, public sf::Transformable {
    public:
        std::array<unsigned int, 115> get_densites() const {return m_densities;};
        static DensityGraph from_song_difficulty(const Data::SongDifficulty& sd);
        static DensityGraph from_time_bounds(const Data::Chart& chart, const Data::TimeBounds& tb);
    private:
        explicit DensityGraph(const std::array<unsigned int, 115>& t_densities);
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        std::array<unsigned int, 115> m_densities;
        sf::VertexArray m_vertex_array;
    };

    using DensityGraphCache = Toolkit::Cache<Data::SongDifficulty, DensityGraph, &DensityGraph::from_song_difficulty>;
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