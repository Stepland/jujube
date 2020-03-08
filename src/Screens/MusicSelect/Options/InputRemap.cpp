#include "InputRemap.hpp"

#include <cmath>

#include <magic_enum.hpp>

#include "../../../Toolkit/HSL.hpp"
#include "../../../Toolkit/NormalizedOrigin.hpp"

namespace MusicSelect {

    PressHere::PressHere(SharedResources& t_resources) :
        HoldsSharedResources(t_resources),
        color_anim(0.0f, 1.0f, 1.0f, 0.1f),
        size_anim(0.0f, 1.0f, 1.0f, 0.7f)
    {
        message.setFont(resources.fallback_font.black);
        message.setString("PRESS\nHERE!");
        message.setFillColor(sf::Color::White);
        message.setCharacterSize(static_cast<unsigned int>(0.2f*get_panel_size()));
        auto bounds = message.getLocalBounds();
        auto biggest_side = std::max(bounds.width, bounds.height);
        if (biggest_side > get_panel_size()*0.8f) {
            message.setScale(get_panel_size()*0.8f / biggest_side, get_panel_size()*0.8f / biggest_side);
        }
    }

    void PressHere::update() {
        message.setCharacterSize(static_cast<unsigned int>(0.2f*get_panel_size()));
        Toolkit::set_local_origin_normalized(message, 0.5f, 0.5f);
        message.setPosition(get_panel_size()*0.5f, get_panel_size()*0.5f);

        auto anim_time = std::fmod(time.getElapsedTime().asSeconds(), 1.f);
        auto color_factor = color_anim.transform(anim_time);
        background.setFillColor(sf::Color(255, 255, 255, static_cast<unsigned int>(128*color_factor)));
        auto size_factor = size_anim.transform(anim_time);
        background.setSize(sf::Vector2f{get_panel_size()*size_factor, get_panel_size()*size_factor});
        Toolkit::set_local_origin_normalized(background, 0.5f, 0.5f);
        background.setPosition(get_panel_size()/2.f, get_panel_size()/2.f);
    }

    void PressHere::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        target.draw(background, states);
        target.draw(message, states);
    }

    AlreadyMapped::AlreadyMapped(SharedResources& t_resources) : HoldsSharedResources(t_resources) {
        background.setFillColor(resources.BSC_color);
    }

    void AlreadyMapped::update() {
        background.setSize(sf::Vector2f{get_panel_size(), get_panel_size()});
    }

    void AlreadyMapped::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        target.draw(background, states);
    }

    MappingPreview::MappingPreview(
        SharedResources& t_resources,
        const std::unordered_map<sf::Keyboard::Key, Data::Button>& t_key_to_button
    ) :
        HoldsSharedResources(t_resources),
        key_to_button(t_key_to_button)
    {
        square.setFillColor(sf::Color::Transparent);
        square.setOutlineThickness(1.f);
        square.setOutlineColor(sf::Color::White);
        key_label.setFillColor(sf::Color::White);
        key_label.setFont(resources.fallback_font.medium);
    }

    void MappingPreview::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        auto gird_size = preferences.layout.big_cover_size*get_screen_width();
        auto square_size = 0.22f*gird_size;
        auto square_step = 0.26f*gird_size;
        square.setSize(sf::Vector2f(square_size, square_size));
        for (size_t i = 0; i < 16; i++) {
            auto column = i%4;
            auto row = i/4;
            square.setPosition(column*square_step, row*square_step);
            target.draw(square, states);
        }

        auto text_size = square_size*0.33f;
        for (auto &&[key, button] : key_to_button) {
            auto coord = Data::button_to_coords(button);
            std::string key_name{magic_enum::enum_name(key)};
            key_label.setString(key_name);
            key_label.setCharacterSize(static_cast<unsigned int>(text_size));
            Toolkit::set_local_origin_normalized(key_label, 0.5f, 0.5f);
            auto bounds = key_label.getLocalBounds();
            auto biggest_side = std::max(bounds.width, bounds.height);
            if (biggest_side > square_size*0.95f) {
                key_label.setScale(square_size*0.95f / biggest_side, square_size*0.95f / biggest_side);
            }
            key_label.setPosition(coord.x*square_step + 0.5f*square_size, coord.y*square_step + 0.5f*square_size);
            target.draw(key_label, states);
        }
    }

    InputRemap::InputRemap(SharedResources& t_resources) :
        OptionPage(t_resources),
        press_here_panel(t_resources),
        already_mapped_panel(t_resources),
        mapping_preview(t_resources, m_key_to_button)
    {
        confirm_text_top.setString("Are you sure about this setting ?");
        confirm_text_top.setFillColor(resources.EXT_color);
        confirm_text_top.setFont(resources.fallback_font.black);
        confirm_text_bottom.setString("Press any key to cancel");
        confirm_text_bottom.setFillColor(resources.EXT_color);
        confirm_text_bottom.setFont(resources.fallback_font.black);
        big_number.setFillColor(sf::Color::Black);
        big_number.setFont(resources.fallback_font.black);
    }

    bool InputRemap::handle_raw_input(const sf::Event::KeyEvent& event) {
        if (next_button < 16) {
            if (m_key_to_button.find(event.code) == m_key_to_button.end()) {
                m_key_to_button[event.code] = *Data::index_to_button(next_button);
                last_key_press.restart();
                next_button++;
                if (next_button == 16) {
                    confirm_clock.emplace();
                }
            }
        } else {
            cancel_remapping = true;
        }
        return true; // we need to capute all input
    }

    bool InputRemap::should_exit() {
        if (cancel_remapping) {
            return true;
        } else {
            if (confirm_clock) {
                return confirm_clock->getElapsedTime() > sf::seconds(5);
            } else {
                return false;
            }
        }
    }

    void InputRemap::exit() {
        if (not cancel_remapping) {
            if (confirm_clock) {
                if (confirm_clock->getElapsedTime() > sf::seconds(5)) {
                    preferences.key_mapping = Data::KeyMapping(m_key_to_button);
                }
            }
        }
        next_button = 0;
        cancel_remapping = false;
        m_key_to_button.clear();
        confirm_clock.reset();
        resources.options_state.pop_back();
    }

    void InputRemap::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        already_mapped_panel.update();
        for (size_t button_index = 0; button_index < next_button; button_index++) {
            already_mapped_panel.setPosition(button_index%4*get_panel_step(), button_index/4*get_panel_step());
            target.draw(already_mapped_panel, states);
        }
        if (next_button < 16) {
            press_here_panel.update();
            press_here_panel.setPosition(next_button%4*get_panel_step(), next_button/4*get_panel_step());
            target.draw(press_here_panel, states);
        }
        auto grid_size = preferences.layout.big_cover_size*get_screen_width();
        mapping_preview.setOrigin(grid_size*0.5f, 0.f);
        mapping_preview.setPosition(get_big_cover_x(), get_big_cover_y());
        target.draw(mapping_preview);
        if (confirm_clock) {
            if (confirm_clock->getElapsedTime() > sf::seconds(5)) {
                preferences.key_mapping = Data::KeyMapping{m_key_to_button};
            }
            std::string seconds = std::to_string(5 - static_cast<int>(confirm_clock->getElapsedTime().asSeconds()));
            big_number.setString(seconds);
            big_number.setCharacterSize(static_cast<unsigned int>(3*get_panel_step()));
            Toolkit::set_local_origin_normalized(big_number, 0.5f, 0.5f);
            big_number.setPosition(get_ribbon_size()*0.5f, get_ribbon_size()*0.5f);
            target.draw(big_number, states);
            auto text_size = 0.1f*grid_size;
            confirm_text_top.setCharacterSize(static_cast<unsigned int>(text_size));
            confirm_text_bottom.setCharacterSize(static_cast<unsigned int>(text_size));
            Toolkit::set_local_origin_normalized(confirm_text_top, 0.5f, 0.f);
            Toolkit::set_local_origin_normalized(confirm_text_bottom, 0.5f, 0.f);
            confirm_text_top.setPosition(get_big_cover_x(), get_big_cover_y()+grid_size*1.1f);
            confirm_text_bottom.setPosition(get_big_cover_x(), get_big_cover_y()+grid_size*1.1f+text_size*1.1f);
            target.draw(confirm_text_top);
            target.draw(confirm_text_bottom);
        }
    }
}
