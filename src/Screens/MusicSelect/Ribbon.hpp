#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include "Panel.hpp"
#include "../../Data/SongList.hpp"
#include "../../Toolkit/AffineTransform.hpp"
#include "../../Toolkit/Debuggable.hpp"
#include "../../Toolkit/EasingFunctions.hpp"

namespace MusicSelect {

    enum class Direction {
        Right,
        Left,
    };

    struct MoveAnimation {
        MoveAnimation(int previous_pos, int next_pos, size_t ribbon_size, Direction direction, float& t_m_time_factor);
        Toolkit::AffineTransform<float> normalized_to_pos;
        Toolkit::AffineTransform<float> seconds_to_normalized;
        float& m_time_factor;
        sf::Clock clock;
        Toolkit::EaseExponential ease_expo;
        float get_position();
        bool ended();
    private:
        Toolkit::AffineTransform<float> create_transform(int previous_pos, int next_pos, size_t ribbon_size, Direction direction);
    };

    // The Ribbon is the moving part of the Music Select Screen
    // It can be sorted in a number of ways
    class Ribbon final : public sf::Drawable, public sf::Transformable, public Toolkit::Debuggable {
    public:
        Ribbon(Resources& t_resources, float& panel_size, float& panel_spacing);
        void title_sort(const Data::SongList& song_list);
        void test_sort();
        void test2_sort();
        void test_song_cover_sort();
        const std::shared_ptr<MusicSelect::Panel>& get_panel_under_button(std::size_t button_index) const;
        void click_on(std::size_t button_index);
        void move_right();
        void move_left();
        void move_to_next_category(const std::size_t& from_button_index);
        void draw_debug() override;
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        void draw_with_animation(sf::RenderTarget& target, sf::RenderStates states) const;
        void draw_without_animation(sf::RenderTarget& target, sf::RenderStates states) const;
        void layout_from_category_map(const std::map<std::string,std::vector<std::shared_ptr<Panel>>>& categories);
        void fill_layout();
        std::size_t get_layout_column(const std::size_t& button_index) const;
        std::vector<std::array<std::shared_ptr<Panel>,3>> m_layout;
        std::size_t m_position = 0;
        mutable std::optional<MoveAnimation> m_move_animation;
        Resources& m_resources;
        float m_time_factor = 1.f;
        Data::Song empty_song;
        float& m_panel_size;
        float& m_panel_spacing;
    };
}