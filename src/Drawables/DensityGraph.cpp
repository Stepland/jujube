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

    DensityGraph compute_density_graph_from_struct(const Data::SongDifficulty& sd) {
        return compute_density_graph_from_song_difficulty(sd.song, sd.difficulty);
    }

    DensityGraph compute_density_graph_from_song_difficulty(const Data::Song& song, const std::string& difficulty) {
        auto c = song.get_chart(difficulty);
        if (not c.has_value()) {
            throw std::invalid_argument("Song "+song.title+" has no "+difficulty+" chart");
        }
        if (c->notes.empty()) {
            return compute_density_graph_from_chart(*c, sf::Time::Zero, sf::Time::Zero);
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
            sf::microseconds(std::min(0LL, c->notes.begin()->timing.asMicroseconds())),
            sf::microseconds(std::max(c->notes.rbegin()->timing.asMicroseconds(), m.getDuration().asMicroseconds()))
        );
    }

    DensityGraph compute_density_graph_from_chart(const Data::Chart& chart, sf::Time start, sf::Time end) {
        std::array<unsigned int, 115> d{};
        if (start != end) {
            Toolkit::AffineTransform<float> ticks_to_column{
                start.asSeconds(),
                end.asSeconds(),
                0.f,
                115.f
            };
            for (auto &&note : chart.notes) {
                auto index = static_cast<unsigned int>(ticks_to_column.transform(note.timing.asSeconds()));
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
    void set_origin_normalized(Drawables::DensityGraph& s, float x, float y) {
        s.setOrigin(x*574.f, y*39.f);
    }
}