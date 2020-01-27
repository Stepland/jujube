#pragma once

#include <SFML/Graphics/Drawable.hpp>

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
        MoveAnimation(int previous_pos, int next_pos, size_t ribbon_size, Direction direction, float& t_time_factor);
        Toolkit::AffineTransform<float> normalized_to_pos;
        Toolkit::AffineTransform<float> seconds_to_normalized;
        float& time_factor;
        sf::Clock clock;
        Toolkit::EaseExponential ease_expo;
        float get_position();
        bool ended();
    private:
        Toolkit::AffineTransform<float> create_transform(int previous_pos, int next_pos, size_t ribbon_size, Direction direction);
    };

    // The Ribbon is the moving part of the Music Select Screen
    // It can be sorted in a number of ways
    class Ribbon final : public sf::Drawable, public Toolkit::Debuggable {
    public:
        Ribbon(const Resources& t_resources) : resources(t_resources) {};
        void title_sort(const Data::SongList& song_list);
        void test_sort();
        void test2_sort();
        const auto& get_layout() {return layout;};
        const std::shared_ptr<MusicSelect::Panel>& at(std::size_t button_index) const;
        void move_right();
        void move_left();
        void move_to_next_category();
        void draw_debug() override;
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        void draw_with_animation(sf::RenderTarget& target, sf::RenderStates states) const;
        void draw_without_animation(sf::RenderTarget& target, sf::RenderStates states) const;
        void layout_from_category_map(const std::map<std::string,std::vector<std::shared_ptr<Panel>>>& categories);
        std::vector<std::array<std::shared_ptr<Panel>,3>> layout;
        std::size_t position = 0;
        mutable std::optional<MoveAnimation> move_animation;
        const Resources& resources;
        float time_factor = 1.f;
    };
}