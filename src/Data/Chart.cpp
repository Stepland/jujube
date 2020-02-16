#include "Chart.hpp"

#include "../Toolkit/AffineTransform.hpp"

#include "Buttons.hpp"

namespace Data {
    Chart::Chart(const stepland::memon& memon, const std::string& chart_name) {
        auto it = memon.charts.find(chart_name);
        if (it == memon.charts.end()) {
            throw std::invalid_argument("Memon file has no "+chart_name+" chart");
        }
        auto [_, chart] = *it;
        level = static_cast<unsigned int>(chart.level);
        resolution = static_cast<std::size_t>(chart.resolution);
        Toolkit::AffineTransform<float> memon_timing_to_300Hz(
            0.f, static_cast<float>(chart.resolution),
            -memon.offset*300.f, (-memon.offset+60.f/memon.BPM)*300.f
        );
        for (auto &&note : chart.notes) {
            auto timing = static_cast<std::size_t>(memon_timing_to_300Hz.transform(note.get_timing()));
            auto position = static_cast<Button>(note.get_pos());
            auto length = static_cast<std::size_t>(note.get_length());
            auto tail = convert_memon_tail(position, note.get_tail_pos());
            notes.emplace(timing, position, length, tail);
        }
    }

    Button convert_memon_tail(Button note, unsigned int tail_position) {
        auto note_position = button_to_index(note);
        assert((note_position >= 0 and note_position <= 15));
        assert((tail_position >= 0 and tail_position <= 11));
        int x = note_position%4;
        int y = note_position/4;
        int dx = 0;
        int dy = 0;
        // Vertical
        if (tail_position%2 == 0) {
            // Going up
            if ((tail_position/2)%2 == 0) {
                dy = -(tail_position/4 + 1);
            // Going down
            } else {
                dy = tail_position/4 +1;
            }
        // Horizontal
        } else {
            // Going right
            if ((tail_position/2)%2 == 0) {
                dx = tail_position/4 + 1;
            // Going left
            } else {
                dx = -(tail_position/4 + 1);
            }
        }
        if (auto tail = coords_to_button({x+dx, y+dy})) {
            return *tail;
        } else {
            throw std::runtime_error("Invalid tail_position : "+tail_position);
        }
    }
}
