#pragma once

#include <SFML/System/Time.hpp>

namespace Data {
    struct TimeBounds {
        sf::Time start = sf::Time::Zero;
        sf::Time end = sf::Time::Zero;

        TimeBounds& operator+=(const TimeBounds& other) {
            if (start > other.start) {
                start = other.start;
            }
            if (end < other.end) {
                end = other.end;
            }
            return *this;
        }
        
        friend TimeBounds operator+(TimeBounds a, const TimeBounds& b) {
            a += b;
            return a;
        };
    };
}