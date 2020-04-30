#pragma once

#include "../Input/Buttons.hpp"

namespace Data {
    struct Note {
        // Timing is stored as ticks on a 1000Hz clock starting at the begging of the audio
        long int timing;
        Input::Button position;
        // zero length means it's a standard note
        std::size_t length;
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