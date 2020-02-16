#pragma once

#include "Buttons.hpp"

namespace Data {
    struct Note {
        // Timing is stored as ticks on a 300Hz clock
        std::size_t timing;
        Button position;
        // zero is standard note
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