#pragma once

#include <array>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include "../../Data/Buttons.hpp"
#include "../../Data/Preferences.hpp"
#include "../../Data/Song.hpp"
#include "../../Toolkit/AffineTransform.hpp"
#include "../../Toolkit/Debuggable.hpp"
#include "../../Toolkit/EasingFunctions.hpp"
#include "SharedResources.hpp"
#include "PanelLayout.hpp"

namespace MusicSelect {

    class Panel;

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

    // A Ribbon is a visual representation of a PanelLayout,
    // You can scroll it using the left and right buttons
    class Ribbon : public sf::Drawable, public sf::Transformable, public HoldsSharedResources, public Toolkit::Debuggable {
    public:
        Ribbon(PanelLayout layout, SharedResources& t_resources);
        jbcoe::polymorphic_value<Panel>& get_panel_under_button(const Data::Button& button);
        void click_on(const Data::Button& button);
        void move_right();
        void move_left();
        void move_to_next_category(const Data::Button& button);
        void draw_debug() override;
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        void draw_with_animation(sf::RenderTarget& target, sf::RenderStates states) const;
        void draw_without_animation(sf::RenderTarget& target, sf::RenderStates states) const;
        std::size_t get_layout_column(const Data::Button& button) const;
        PanelLayout m_layout;
        std::size_t m_position = 0;
        mutable std::optional<MoveAnimation> m_move_animation;
        float m_time_factor = 1.f;
    };
}