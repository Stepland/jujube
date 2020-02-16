#pragma once

#include <cstddef>
#include <set>

#include <memon.hpp>

#include "Buttons.hpp"
#include "Note.hpp"

namespace Data {
    struct Chart {
        explicit Chart(const stepland::memon& memon, const std::string& chart);
        unsigned int level;
        std::set<Note> notes;
        std::size_t resolution;
    };
    Button convert_memon_tail(Button note, unsigned int tail_position);
}