#pragma once

#include "Buttons.hpp"

namespace Data {
    struct Note {
        // Timing is stored as ticks on a 300Hz clock starting at the begging of the audio
        long int timing;
        Button position;
        // zero length means it's a standard note
        std::size_t length;
        Button tail;

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