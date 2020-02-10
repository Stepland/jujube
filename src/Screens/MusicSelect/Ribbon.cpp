#include "Ribbon.hpp"

#include "imgui/imgui.h"
#include "imgui-sfml/imgui-SFML.h"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <map>
#include <vector>

#include "Panel.hpp"
#include "../../Data/SongList.hpp"
#include "../../Toolkit/QuickRNG.hpp"


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

    Ribbon::Ribbon(Resources& t_resources, float& panel_size, float& panel_spacing) :
        m_layout(),
        m_move_animation(),
        m_resources(t_resources),
        empty_song(),
        m_panel_size(panel_size),
        m_panel_spacing(panel_spacing)
    {
        std::cout << "Loaded MusicSelect::Ribbon" << std::endl;
    }

    void Ribbon::title_sort(const Data::SongList &song_list) {
        std::vector<std::reference_wrapper<const Data::Song>> songs;
        for (auto &&song : song_list.songs) {
            songs.push_back(std::cref(song));
        }
        std::sort(songs.begin(), songs.end(), Data::Song::sort_by_title);
        std::map<std::string, std::vector<std::shared_ptr<Panel>>> categories;
        for (const auto &song : songs) {
            if (song.get().title.size() > 0) {
                char letter = song.get().title[0];
                if ('A' <= letter and letter <= 'Z') {
                    categories
                    [std::string(1, letter)]
                    .push_back(
                        std::make_shared<SongPanel>(m_panel_size, m_resources, song)
                    );
                } else if ('a' <= letter and letter <= 'z') {
                    categories
                    [std::string(1, 'A' + (letter - 'a'))]
                    .push_back(
                        std::make_shared<SongPanel>(m_panel_size, m_resources, song)
                    );
                } else {
                    categories["?"].push_back(std::make_shared<SongPanel>(m_panel_size, m_resources, song));
                }
            } else {
                categories["?"].push_back(std::make_shared<SongPanel>(m_panel_size, m_resources, song));
            }
        }
        layout_from_category_map(categories);
    }

    void Ribbon::test_sort() {
        m_layout.clear();
        m_layout.push_back({
            std::make_shared<EmptyPanel>(m_panel_size, m_resources),
            std::make_shared<CategoryPanel>(m_panel_size, m_resources, "A"),
            std::make_shared<CategoryPanel>(m_panel_size, m_resources, "truc")
        });
        for (size_t i = 0; i < 3; i++) {
            m_layout.push_back({
                std::make_shared<EmptyPanel>(m_panel_size, m_resources),
                std::make_shared<EmptyPanel>(m_panel_size, m_resources),
                std::make_shared<EmptyPanel>(m_panel_size, m_resources)
            });
        }
        fill_layout();
    }

    void Ribbon::test2_sort() {
        std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        std::map<std::string, std::vector<std::shared_ptr<Panel>>> categories;
        Toolkit::UniformIntRNG category_size_generator{1, 10};
        Toolkit::UniformIntRNG panel_hue_generator{0, 255};
        for (auto &&letter : alphabet) {
            auto category_size = category_size_generator.generate();
            for (int i = 0; i < category_size; i++) {
                categories[std::string(1, letter)].push_back(
                    std::make_shared<ColorPanel>(
                        m_panel_size, m_resources,
                        sf::Color(
                            panel_hue_generator.generate(),
                            panel_hue_generator.generate(),
                            panel_hue_generator.generate()
                        )
                    )
                );
            }
        }
        layout_from_category_map(categories);
    }

    void Ribbon::test_song_cover_sort() {
        std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        std::map<std::string, std::vector<std::shared_ptr<Panel>>> categories;
        Toolkit::UniformIntRNG category_size_generator{1, 10};
        for (auto &&letter : alphabet) {
            auto category_size = category_size_generator.generate();
            for (int i = 0; i < category_size; i++) {
                categories[std::string(1, letter)].push_back(
                    std::make_shared<SongPanel>(m_panel_size, m_resources, this->empty_song)
                );
            }
        }
        layout_from_category_map(categories);
    }


    std::size_t Ribbon::get_layout_column(const std::size_t& button_index) const {
        return (m_position + (button_index % 4)) % m_layout.size();
    }

    const std::shared_ptr<Panel> &Ribbon::get_panel_under_button(std::size_t button_index) const {
        return (
            m_layout
            .at(this->get_layout_column(button_index))
            .at(button_index / 4)
        );
    }

    void Ribbon::click_on(std::size_t button_index) {
        this->get_panel_under_button(button_index)->click(*this, button_index);
    }

    void Ribbon::layout_from_category_map(const std::map<std::string, std::vector<std::shared_ptr<Panel>>> &categories) {
        m_layout.clear();
        for (auto &&[category, panels] : categories) {
            if (not panels.empty()) {
                std::vector<std::shared_ptr<Panel>> current_column;
                current_column.push_back(std::make_shared<CategoryPanel>(m_panel_size, m_resources, category));
                for (auto &&panel : panels) {
                    if (current_column.size() == 3) {
                        m_layout.push_back({current_column[0], current_column[1], current_column[2]});
                        current_column.clear();
                    } else {
                        current_column.push_back(std::move(panel));
                    }
                }
                if (not current_column.empty()) {
                    while (current_column.size() < 3) {
                        current_column.push_back(std::make_shared<EmptyPanel>(m_panel_size, m_resources));
                    }
                    m_layout.push_back({current_column[0], current_column[1], current_column[2]});
                }
            }
        }
        fill_layout();
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

    void Ribbon::move_to_next_category(const std::size_t& from_button_index) {
        std::size_t old_position = m_position;
        std::size_t from_column = this->get_layout_column(from_button_index);

        bool found = false;
        size_t offset = 1;
        // Cycle through the whole ribbon once starting on the column next to
        // the one that was just clicked, possibly wrapping around
        while(offset < m_layout.size()) {
            const auto& column = m_layout.at((from_column + offset) % m_layout.size());
            if (std::any_of(
                column.begin(),
                column.end(),
                [](std::shared_ptr<Panel> panel) -> bool {
                    return (std::dynamic_pointer_cast<CategoryPanel>(panel).get() != nullptr);
                }
            )) {
                found = true;
                break;
            }
            offset++;
        }
        if (found) {
            // we want the next category panel to land on the same column we clicked
            auto next_category_column = from_column + offset;
            auto onscreen_clicked_column = (from_button_index % 4);
            m_position = next_category_column - onscreen_clicked_column;
            m_move_animation.emplace(old_position, m_position, m_layout.size(), Direction::Right, m_time_factor);
        }
    }

    void Ribbon::draw(sf::RenderTarget &target, sf::RenderStates states) const {
        states.transform *= getTransform();
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
                auto panel = m_layout.at(actual_column).at(row);
                panel->setPosition(
                    (static_cast<float>(relative_column_zero + column_offset) - float_position) * (m_panel_size+m_panel_spacing),
                    row * (m_panel_size+m_panel_spacing)
                );
                target.draw(*panel, states);
            }
        }
    }

    void Ribbon::draw_without_animation(sf::RenderTarget &target, sf::RenderStates states) const {
        for (int column = -1; column <= 4; column++) {
            int actual_column_index = (column + m_position + m_layout.size()) % m_layout.size();
            for (int row = 0; row < 3; row++) {
                auto panel = m_layout.at(actual_column_index).at(row);
                panel->setPosition(column * (m_panel_size+m_panel_spacing), row * (m_panel_size+m_panel_spacing));
                target.draw(*panel, states);
            }
        }
    }

    void Ribbon::draw_debug() {
        if (debug) {
            ImGui::Begin("Ribbon Debug", &debug); {
                ImGui::SliderFloat("Time Slowdown Factor", &m_time_factor, 1.f, 10.f);
                if (ImGui::CollapsingHeader("Panels")) {
                    auto panel_size = static_cast<int>(m_panel_size);
                    if(ImGui::InputInt("Size", &panel_size)) {
                        if (panel_size < 0) {
                            panel_size = 0;
                        }
                        m_panel_size = static_cast<std::size_t>(panel_size);
                    }
                    auto panel_spacing = static_cast<int>(m_panel_spacing);
                    if(ImGui::InputInt("Spacing", &panel_spacing)) {
                        if (panel_spacing < 0) {
                            panel_spacing = 0;
                        }
                        m_panel_spacing = static_cast<std::size_t>(panel_spacing);
                    }
                }
            }
            ImGui::End();
        }
    }

    // Obligatory steps before the drawing functions can use the layout without crashing
    void Ribbon::fill_layout() {
        if (m_layout.empty()) {
            m_layout.push_back({
                std::make_shared<ColoredMessagePanel>(m_panel_size, m_resources, sf::Color::Red, "- EMPTY -"),
                std::make_shared<ColoredMessagePanel>(m_panel_size, m_resources, sf::Color::Red, "- EMPTY -"),
                std::make_shared<ColoredMessagePanel>(m_panel_size, m_resources, sf::Color::Red, "- EMPTY -"),
            });
            m_layout.push_back({
                std::make_shared<ColoredMessagePanel>(m_panel_size, m_resources, sf::Color::Red, "- EMPTY -"),
                std::make_shared<ColoredMessagePanel>(m_panel_size, m_resources, sf::Color::Red, "- EMPTY -"),
                std::make_shared<ColoredMessagePanel>(m_panel_size, m_resources, sf::Color::Red, "- EMPTY -"),
            });
            return;
        }
        while (m_layout.size() < 4) {
            m_layout.push_back({
                std::make_shared<EmptyPanel>(m_panel_size, m_resources),
                std::make_shared<EmptyPanel>(m_panel_size, m_resources),
                std::make_shared<EmptyPanel>(m_panel_size, m_resources),
            });
        }
    }
}
