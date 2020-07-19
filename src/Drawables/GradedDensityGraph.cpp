#include "GradedDensityGraph.hpp"

#include <algorithm>

#include <imgui/imgui.h>
#include <imgui-sfml/imgui-SFML.h>
#include <SFML/System/Time.hpp>

namespace Drawables {
    DensityLineGrade judgement_to_density_line_grade(Data::Judgement judge) {
        switch (judge) {
        case Data::Judgement::Perfect:
            return DensityLineGrade::Perfect;
        case Data::Judgement::Good:
        case Data::Judgement::Great:
            return DensityLineGrade::Great;
        default:
            return DensityLineGrade::ComboBreak;
        }
    }
    
    DensityLineGrade merge_grades(DensityLineGrade current, DensityLineGrade _new) {
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
        default:
            throw std::runtime_error("wtf ?");
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
        default:
            throw std::runtime_error("wtf ?");
        }
    }

    Toolkit::AffineTransform<float> get_seconds_to_column_transform(const Data::SongDifficulty& sd) {
        auto bounds = sd.get_time_bounds();
        return Toolkit::AffineTransform<float>(
            bounds.start.asSeconds(),
            bounds.end.asSeconds(),
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
    }

    void GradedDensityGraph::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        target.draw(&m_vertex_array[0], m_vertex_array.size(), sf::Quads, states);
        if (debug) {
            draw_debug();
        }
    }

    void GradedDensityGraph::update(const sf::Time& music_time) {
        const auto float_column = m_seconds_to_column.transform(music_time.asSeconds());
        const auto current_column = static_cast<std::size_t>(float_column);
        const auto current_column_it = std::next(m_densities.begin(), current_column);
        for (auto it = m_densities.begin(); it != current_column_it and it != m_densities.end(); ++it) {
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
    }

    void GradedDensityGraph::update_grades(const Data::Judgement& judge, const sf::Time& timing) {
        const auto float_column = m_seconds_to_column.transform(timing.asSeconds());
        const auto column = static_cast<std::size_t>(float_column);
        auto& current_grade = m_densities.at(column).grade;
        current_grade = merge_grades(current_grade, judgement_to_density_line_grade(judge));
    }

    sf::FloatRect GradedDensityGraph::getLocalBounds() const {
        return sf::FloatRect({0, 0}, {(115*5)-1, 39});
    }

    sf::FloatRect GradedDensityGraph::getGlobalBounds() const {
        return getTransform().transformRect(getLocalBounds());
    }

    void Drawables::GradedDensityGraph::draw_debug() const {
        if (ImGui::Begin("GradedDensityGraph")) {
            auto orig_x = ImGui::GetCursorPosX();
            auto pos = ImGui::GetCursorPos();
            for (size_t line = 0; line < m_densities.size(); line++) {
                const auto& graded_density = m_densities.at(line);
                auto color = grade_to_color(graded_density.grade);
                for (size_t column = 0; column < graded_density.density; column++) {
                    ImGui::SetCursorPos(pos);
                    ImGui::DrawRectFilled({{0,0}, {4,4}}, color);
                    pos.x += 5;
                }
                pos.x = orig_x;
                pos.y += 5;
            }
            ImGui::Dummy({8*5, 115*5});
        }
        ImGui::End();
    }
}
