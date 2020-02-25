#pragma once

#include <cstddef>
#include <set>

#include <memon/memon.hpp>

#include "Buttons.hpp"
#include "Note.hpp"

namespace Data {
    struct Chart {
        Chart(const stepland::memon& memon, const std::string& difficulty);
        int level;
        std::set<Note> notes;
        std::size_t resolution;
    };
    Button convert_memon_tail(Button note, unsigned int tail_position);
}