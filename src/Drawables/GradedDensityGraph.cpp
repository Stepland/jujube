#include "GradedDensityGraph.hpp"

#include <algorithm>

#include <SFML/System/Time.hpp>

namespace Drawables {

    DensityLineGrade judgement_to_density_line_grade(Data::Judgement judge) {
        switch (judge) {
        case Data::Judgement::Perfect:
            return DensityLineGrade::Perfect;
        case Data::Judgement::Great:
            return DensityLineGrade::Great;
        default:
            return DensityLineGrade::ComboBreak;
        }
    }
    
    DensityLineGrade update(DensityLineGrade current, DensityLineGrade _new) {
        switch (current) {
        case DensityLineGrade::Perfect:
            return _new;
        case DensityLineGrade::Great:
            switch (_new) {
            case DensityLineGrade::ComboBreak:
                return _new;
            default:
                return current;
            }
        case DensityLineGrade::ComboBreak:
            return current;
        case DensityLineGrade::NonGraded:
            return _new;
        }
    }

    sf::Color grade_to_color(DensityLineGrade grade) {
        switch (grade) {
        case DensityLineGrade::Perfect:
            return sf::Color(248, 249, 91);
        case DensityLineGrade::Great:
            return sf::Color(59, 152, 230);
        case DensityLineGrade::ComboBreak:
            return sf::Color(99, 101, 133);
        case DensityLineGrade::NonGraded:
            return sf::Color::White;
        }
    }

    Toolkit::AffineTransform<float> get_seconds_to_column_transform_from_notes(const Data::Chart& chart) {
        auto time_bounds = chart.get_time_bounds_from_notes();
        // only one timing point
        if (time_bounds.start == time_bounds.end) {
            // give one extra second
            return Toolkit::AffineTransform<float>(
                time_bounds.start.asSeconds(),
                (time_bounds.start + sf::seconds(1)).asSeconds(),
                0.f,
                115.f
            );
        } else {
            // at least two timing points
            return Toolkit::AffineTransform<float>(
                time_bounds.start.asSeconds(),
                time_bounds.end.asSeconds(),
                0.f,
                115.f
            );
        }
    }

    Toolkit::AffineTransform<float> get_seconds_to_column_transform(const Data::SongDifficulty& sd) {
        auto chart = sd.song.get_chart(sd.difficulty);
        if (not chart) {
            throw std::invalid_argument("Song "+sd.song.title+" has no '"+sd.difficulty+"' chart");
        }
        // no notes, no need to return something useful
        if (chart->notes.empty()) {
            return Toolkit::AffineTransform<float>(0.f, 1.f, 0.f, 115.f);
        }
        // no audio
        if (not sd.song.audio) {
            return get_seconds_to_column_transform_from_notes(*chart);
        }
        sf::Music m;
        // can't open audio
        if (not m.openFromFile(*sd.song.full_audio_path())) {
            return get_seconds_to_column_transform_from_notes(*chart);
        }
        // both notes and audio exist
        auto time_bounds = chart->get_time_bounds_from_notes();
        return Toolkit::AffineTransform<float>(
            time_bounds.start.asSeconds(),
            sf::microseconds(
                std::max(
                    time_bounds.end.asMicroseconds(),
                    m.getDuration().asMicroseconds()
                )
            ).asSeconds(),
            0.f,
            115.f
        );
    }

    GradedDensityGraph::GradedDensityGraph(const DensityGraph& density_graph, const Data::SongDifficulty& sd) :
        m_seconds_to_column(get_seconds_to_column_transform(sd))
    {
        const auto raw_densites = density_graph.get_densites();
        std::transform(
            raw_densites.begin(),
            raw_densites.end(),
            m_densities.begin(),
            [](unsigned int i) -> GradedDensity {
                return GradedDensity{i};
            }
        );
        std::size_t column = 0;
        sf::Vector2f origin{0.f, 39.f};
        std::vector<std::size_t> index_of_first_vertex_of_each_column;
        for (auto &&colored_density : m_densities) {
            index_of_first_vertex_of_each_column.push_back(m_vertex_array.size());
            for (size_t row = 0; row < static_cast<std::size_t>(colored_density.density); row++) {
                m_vertex_array.emplace_back(
                    origin+sf::Vector2f(column*5.0f, row*-5.0f),
                    sf::Color::White
                );
                m_vertex_array.emplace_back(
                    origin+sf::Vector2f(column*5.0f-4.0f, row*-5.0f),
                    sf::Color::White
                );
                m_vertex_array.emplace_back(
                    origin+sf::Vector2f(column*5.0f-4.0, row*-5.0f-4.0f),
                    sf::Color::White
                );
                m_vertex_array.emplace_back(
                    origin+sf::Vector2f(column*5.0f, row*-5.0f-4.0f),
                    sf::Color::White
                );
            }
            column++;
        }
        auto it = m_vertex_array.begin();
        for (size_t i = 0; i < index_of_first_vertex_of_each_column.size(); i++) {
            m_densities.at(i).first_vertex = std::next(it, index_of_first_vertex_of_each_column.at(i));
        }
        first_non_played_density = m_densities.begin();
    }

    void GradedDensityGraph::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        target.draw(&m_vertex_array[0], m_vertex_array.size(), sf::Quads, states);
    }

    void GradedDensityGraph::update_colors(const sf::Time& music_time) {
        const auto float_column = m_seconds_to_column.transform(music_time.asSeconds());
        const auto current_column = static_cast<std::size_t>(float_column);
        const auto current_column_it = std::next(m_densities.begin(), current_column);
        for (auto it = first_non_played_density; it != current_column_it; ++it) {
            auto color = grade_to_color(it->grade);
            auto next = std::next(it);
            std::vector<sf::Vertex>::iterator next_vertex_it;
            if (next == m_densities.end()) {
                next_vertex_it = m_vertex_array.end();
            } else {
                next_vertex_it = next->first_vertex;
            }
            for (auto vertex_it = it->first_vertex; vertex_it != next->first_vertex; ++vertex_it) {
                vertex_it->color = color;
            }
        }
        first_non_played_density = current_column_it;
    }

    void GradedDensityGraph::update_grades(const Data::Judgement& judge, const sf::Time& timing) {
        const auto float_column = m_seconds_to_column.transform(timing.asSeconds());
        const auto column = static_cast<std::size_t>(float_column);
        auto& current_grade = m_densities.at(column).grade;
        current_grade = update(current_grade, judgement_to_density_line_grade(judge));
    }

}

namespace Toolkit {
    template<>
    void set_origin_normalized(Drawables::GradedDensityGraph& s, float x, float y) {
        s.setOrigin(x*574.f, y*39.f);
    }
}
