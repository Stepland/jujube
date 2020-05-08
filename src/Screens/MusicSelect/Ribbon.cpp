#include "Ribbon.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <map>
#include <memory>
#include <vector>

#include <imgui/imgui.h>
#include <imgui-sfml/imgui-SFML.h>

#include "../../Data/Song.hpp"
#include "../../Toolkit/QuickRNG.hpp"
#include "Panels/Panel.hpp"


namespace MusicSelect {
    MoveAnimation::MoveAnimation(int previous_pos, int next_pos, size_t ribbon_size, Direction direction, float &t_time_factor) : 
        normalized_to_pos(create_transform(previous_pos, next_pos, ribbon_size, direction)),
        seconds_to_normalized(0.f, 0.3f, 0.f, 1.f),
        m_time_factor(t_time_factor),
        clock(),
        ease_expo(-7.f)
    {
    }

    Toolkit::AffineTransform<float> MoveAnimation::create_transform(int previous_pos, int next_pos, size_t ribbon_size, Direction direction) {
        // We first deal with cases were we cross the end of the ribbon
        if (direction == Direction::Left and next_pos > previous_pos) {
            return Toolkit::AffineTransform<float>(
                0.f,
                1.f,
                static_cast<float>(previous_pos),
                static_cast<float>(next_pos) - ribbon_size);
        }
        else if (direction == Direction::Right and next_pos < previous_pos) {
            return Toolkit::AffineTransform<float>(
                0.f,
                1.f,
                static_cast<float>(previous_pos),
                static_cast<float>(next_pos) + ribbon_size
            );
        } else {
            return Toolkit::AffineTransform<float>(
                0.f,
                1.f,
                static_cast<float>(previous_pos),
                static_cast<float>(next_pos)
            );
        }
    }

    float MoveAnimation::get_position() {
        return normalized_to_pos.transform(
            ease_expo.transform(
                seconds_to_normalized.clampedTransform(
                    clock.getElapsedTime().asSeconds() / m_time_factor
                )
            )
        );
    }

    bool MoveAnimation::ended() {
        return clock.getElapsedTime() / m_time_factor > sf::milliseconds(300);
    }

    Ribbon::Ribbon(PanelLayout layout, ScreenResources& t_resources) :
        HoldsResources(t_resources),
        m_layout(layout),
        left_button(t_resources),
        right_button(t_resources)
    {
        std::cout << "Loaded MusicSelect::Ribbon" << '\n';
    }


    std::size_t Ribbon::get_layout_column(const Input::Button& button) const {
        return (m_position + (Input::button_to_index(button) % 4)) % m_layout.size();
    }

    std::shared_ptr<Panel>& Ribbon::get_panel_under_button(const Input::Button& button) {
        auto button_index = Input::button_to_index(button);
        return (
            m_layout
            .at(this->get_layout_column(button))
            .at(button_index / 4)
        );
    }

    void Ribbon::click_on(const Input::Button& button) {
        switch (button) {
        case Input::Button::B13: // Left Arrow
            move_left();
            break;
        case Input::Button::B14: // Right Arrow
            move_right();
            break;
        default:
            get_panel_under_button(button)->click(*this, button);
            break;
        }
    }

    void Ribbon::move_right() {
        std::size_t old_position = m_position;
        m_position = (m_position + 1) % m_layout.size();
        m_move_animation.emplace(old_position, m_position, m_layout.size(), Direction::Right, m_time_factor);
    }

    void Ribbon::move_left() {
        std::size_t old_position = m_position;
        if (m_position == 0) {
            m_position = m_layout.size() - 1;
        } else {
            m_position--;
        }
        m_move_animation.emplace(old_position, m_position, m_layout.size(), Direction::Left, m_time_factor);
    }

    void Ribbon::move_to_next_category(const Input::Button& button) {
        std::size_t from_column = this->get_layout_column(button);
        bool found = false;
        size_t offset = 1;
        // Cycle through the whole ribbon once starting on the column next to
        // the one that was just clicked, possibly wrapping around
        while(offset < m_layout.size()) {
            const auto& column = m_layout.at((from_column + offset) % m_layout.size());
            if (std::any_of(
                column.begin(),
                column.end(),
                [](const std::shared_ptr<Panel>& panel) -> bool {
                    return std::dynamic_pointer_cast<CategoryPanel>(panel) != nullptr;
                }
            )) {
                found = true;
                break;
            }
            offset++;
        }
        if (found) {
            // we want the next category panel to land on the same column we clicked
            auto old_position = m_position;
            auto button_index = Input::button_to_index(button);
            auto next_category_column = from_column + offset;
            auto onscreen_clicked_column = (button_index % 4);
            m_position = next_category_column - onscreen_clicked_column;
            m_move_animation.emplace(old_position, m_position, m_layout.size(), Direction::Right, m_time_factor);
        }
    }

    void Ribbon::draw(sf::RenderTarget &target, sf::RenderStates states) const {
        states.transform *= getTransform();

        left_button.setPosition(0.f, 3.f*get_panel_step());
        target.draw(left_button, states);
        right_button.setPosition(get_panel_step(), 3.f*get_panel_step());
        target.draw(right_button, states);
        
        if (m_move_animation) {
            if (not m_move_animation->ended()) {
                return draw_with_animation(target, states);
            } else {
                m_move_animation.reset();
            }
        }
        draw_without_animation(target, states);
    }

    void Ribbon::draw_with_animation(sf::RenderTarget &target, sf::RenderStates states) const {
        auto float_position = m_move_animation->get_position();
        int relative_column_zero = static_cast<int>(std::floor(float_position));
        std::size_t column_zero = (relative_column_zero + m_layout.size()) % m_layout.size();

        if (debug) {
            if (ImGui::Begin("Ribbon Debug")) {
                ImGui::Text("float position : %f", float_position);
                ImGui::Text("zeroth column : %lu", column_zero);
            }
            ImGui::End();
        }
        for (int column_offset = -1; column_offset <= 4; column_offset++) {
            std::size_t actual_column = (column_zero + column_offset + m_layout.size()) % m_layout.size();
            for (int row = 0; row < 3; row++) {
                auto& panel = m_layout.at(actual_column).at(row);
                panel->setPosition(
                    (static_cast<float>(relative_column_zero + column_offset) - float_position) * (get_panel_step()),
                    row * (get_panel_step())
                );
                target.draw(*panel, states);
            }
        }
    }

    void Ribbon::draw_without_animation(sf::RenderTarget &target, sf::RenderStates states) const {
        for (int column = -1; column <= 4; column++) {
            int actual_column_index = (column + m_position + m_layout.size()) % m_layout.size();
            for (int row = 0; row < 3; row++) {
                auto& panel = m_layout.at(actual_column_index).at(row);
                panel->setPosition(column * (get_panel_step()), row * (get_panel_step()));
                target.draw(*panel, states);
            }
        }
    }

    void Ribbon::draw_debug() {
        if (debug) {
            ImGui::Begin("Ribbon Debug", &debug); {
                ImGui::SliderFloat("Time Slowdown Factor", &m_time_factor, 1.f, 10.f);
            }
            ImGui::End();
        }
    }
}
