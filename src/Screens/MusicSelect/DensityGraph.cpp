#include "DensityGraph.hpp"

#include <algorithm>
#include <functional>

#include <SFML/Audio.hpp>

#include "../../Toolkit/AffineTransform.hpp"

namespace MusicSelect {

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

    DensityGraph compute_density_graph_from_struct(const SongDifficulty& sd) {
        return compute_density_graph_from_song(sd.song, sd.difficulty);
    }

    DensityGraph compute_density_graph_from_song(const Data::Song& song, const std::string& difficulty) {
        auto c = song.get_chart(difficulty);
        if (not c.has_value()) {
            throw std::invalid_argument("Song "+song.title+" has no "+difficulty+" chart");
        }
        if (c->notes.empty()) {
            return compute_density_graph_from_chart(*c, 0, 0);
        }
        if (not song.audio.has_value()) {
            return compute_density_graph_from_chart(*c, c->notes.begin()->timing, c->notes.rbegin()->timing);
        }
        sf::Music m;
        if (not m.openFromFile(*song.full_audio_path())) {
            return compute_density_graph_from_chart(*c, c->notes.begin()->timing, c->notes.rbegin()->timing);
        }
        return compute_density_graph_from_chart(
            *c,
            std::min(0L, c->notes.begin()->timing),
            std::max(c->notes.rbegin()->timing, static_cast<long>(m.getDuration().asMilliseconds()*3/10))
        );
    }

    DensityGraph compute_density_graph_from_chart(const Data::Chart& chart, long start, long end) {
        std::array<unsigned int, 115> d{};
        if (start != end) {
            Toolkit::AffineTransform<float> ticks_to_column{
                static_cast<float>(start),
                static_cast<float>(end),
                0.f,
                115.f
            };
            for (auto &&note : chart.notes) {
                auto index = static_cast<unsigned int>(ticks_to_column.transform(static_cast<float>(note.timing)));
                d.at(index) += 1;
            }
            std::replace_if(
                d.begin(),
                d.end(),
                std::bind(std::greater<unsigned int>(), std::placeholders::_1, 8U),
                8
            );
        }
        return DensityGraph{d};
    }
}

namespace Toolkit {
    template<>
    void set_origin_normalized(MusicSelect::DensityGraph& s, float x, float y) {
        s.setOrigin(x*574.f, y*39.f);
    }
}