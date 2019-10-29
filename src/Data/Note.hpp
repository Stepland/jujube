#pragma once

#include <SFML/System.hpp>

namespace Data {
    class Note {
    public:
        Note(unsigned int t_position, sf::Time t_timing, sf::Time t_length, unsigned int t_tail_position);

        bool operator==(const Note &rhs) const;
        bool operator!=(const Note &rhs) const;
        bool operator<(const Note &rhs) const;
        bool operator>(const Note &rhs) const;
        bool operator<=(const Note &rhs) const;
        bool operator>=(const Note &rhs) const;

        const unsigned int getPosition() const;
        const sf::Time& getTiming() const;
        const sf::Time& getLength() const;
        const unsigned int getTailPosition() const;

    private:

        const unsigned int position;
        const sf::Time timing;
        const sf::Time length = sf::Time{};
        const unsigned int tail_position = 0;

    };
};