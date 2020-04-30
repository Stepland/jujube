#pragma once

#include <SFML/System/Time.hpp>

#include "../Input/Buttons.hpp"

namespace Data {
    struct Note {
        // offset since the begging of the audio
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
    };
}