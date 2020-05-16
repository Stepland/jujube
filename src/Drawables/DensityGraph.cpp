#include "DensityGraph.hpp"

#include <algorithm>
#include <functional>

#include <SFML/Audio.hpp>

#include "../Toolkit/AffineTransform.hpp"

namespace Drawables {

    DensityGraph::DensityGraph(const std::array<unsigned int, 115>& t_densities) :
        m_densities(t_densities),
        m_vertex_array(sf::Quads, 0)
    {
        std::size_t column = 0;
        sf::Vector2f origin{0.f, 39.f};
        for (auto &&density : m_densities) {
            for (size_t row = 0; row < static_cast<std::size_t>(density); row++) {
                m_vertex_array.append(sf::Vertex(
                    origin+sf::Vector2f(column*5.0f, row*-5.0f),
                    sf::Color::White
                ));
                m_vertex_array.append(sf::Vertex(
                    origin+sf::Vector2f(column*5.0f-4.0f, row*-5.0f),
                    sf::Color::White
                ));
                m_vertex_array.append(sf::Vertex(
                    origin+sf::Vector2f(column*5.0f-4.0, row*-5.0f-4.0f),
                    sf::Color::White
                ));
                m_vertex_array.append(sf::Vertex(
                    origin+sf::Vector2f(column*5.0f, row*-5.0f-4.0f),
                    sf::Color::White
                ));
            }
            column++;
        }
    }

    void DensityGraph::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        target.draw(m_vertex_array, states);
    }

    DensityGraph DensityGraph::from_song_difficulty(const Data::SongDifficulty& sd) {
        return DensityGraph::from_time_bounds(*sd.song.get_chart(sd.difficulty), sd.get_time_bounds());
    }

    DensityGraph DensityGraph::from_time_bounds(const Data::Chart& chart, const Data::TimeBounds& tb) {
        std::array<unsigned int, 115> d{};
        Toolkit::AffineTransform<float> ticks_to_column{
            tb.start.asSeconds(),
            tb.end.asSeconds(),
            0.f,
            115.f
        };
        for (auto &&note : chart.notes) {
            auto index = static_cast<std::size_t>(ticks_to_column.transform(note.timing.asSeconds()));
            d.at(index) += 1;
            if (note.duration > sf::Time::Zero) {
                auto long_note_end_index = static_cast<std::size_t>(
                    ticks_to_column.transform((note.timing+note.duration).asSeconds())
                );
                d.at(long_note_end_index) += 1;
            }
        }
        std::replace_if(
            d.begin(),
            d.end(),
            std::bind(std::greater<unsigned int>(), std::placeholders::_1, 8U),
            8
        );
        return DensityGraph{d};
    }
}

namespace Toolkit {
    template<>
    void set_origin_normalized(Drawables::DensityGraph& s, float x, float y) {
        s.setOrigin(x*574.f, y*39.f);
    }
}
