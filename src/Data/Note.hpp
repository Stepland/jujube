#pragma once

#include <cassert>

#include <SFML/System/Time.hpp>

#include "../Input/Buttons.hpp"

namespace Data {
    struct Note {
        // offset since the beginning of the audio
        sf::Time timing;
        Input::Button position;
        // zero means it's a normal note
        sf::Time duration;
        Input::Button tail;

        bool operator==(const Note &rhs) const {
            return timing == rhs.timing && position == rhs.position;
        };
        bool operator!=(const Note &rhs) const {
            return not(rhs == *this);
        };
        bool operator<(const Note &rhs) const {
            if (timing < rhs.timing) {
                return true;
            }
            if (rhs.timing < timing) {
                return false;
            }
            return position < rhs.position;
        };
        bool operator>(const Note &rhs) const {
            return rhs < *this;
        };
        bool operator<=(const Note &rhs) const {
            return !(rhs < *this);
        };
        bool operator>=(const Note &rhs) const {
            return !(*this < rhs);
        };

        // get the integer distance between the tail tip starting position
        // and the target square
        int get_tail_length() const {
            auto pos_coords = Input::button_to_coords(position);
            auto tail_coords = Input::button_to_coords(tail);
            assert(pos_coords.x == tail_coords.x or pos_coords.y == tail_coords.y);
            if (pos_coords.x == tail_coords.x) {
                return std::abs(static_cast<int>(pos_coords.y)-static_cast<int>(tail_coords.y));
            } else {
                return std::abs(static_cast<int>(pos_coords.x)-static_cast<int>(tail_coords.x));
            }
        }

        // returns the number of clockwise quarter turns needed to rotate
        // a tail going down to get the tail's actual orienation
        int get_tail_angle() const {
            auto pos_coords = Input::button_to_coords(position);
            auto tail_coords = Input::button_to_coords(tail);
            assert(pos_coords.x == tail_coords.x or pos_coords.y == tail_coords.y);
            if (pos_coords.x == tail_coords.x) {
                if (tail_coords.y > pos_coords.y) {
                    return 2;
                } else {
                    return 0;
                }
            } else {
                if (tail_coords.x > pos_coords.x) {
                    return 1;
                } else {
                    return 3;
                }
            }
        }
    };
}