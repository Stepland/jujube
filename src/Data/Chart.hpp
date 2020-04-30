#pragma once

#include <cstddef>
#include <set>

#include <memon/memon.hpp>

#include "../Input/Buttons.hpp"
#include "Note.hpp"

namespace Data {
    struct Chart {
        Chart(const stepland::memon& memon, const std::string& difficulty);
        int level;
        std::set<Note> notes;
        std::size_t resolution;
    };
    Input::Button convert_memon_tail(Input::Button note, unsigned int tail_position);
}