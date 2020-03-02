#include "Chart.hpp"

#include "../Toolkit/AffineTransform.hpp"

#include "Buttons.hpp"

namespace Data {
    Chart::Chart(const stepland::memon& memon, const std::string& difficulty) {
        auto it = memon.charts.find(difficulty);
        if (it == memon.charts.end()) {
            throw std::invalid_argument("Memon file has no "+difficulty+" chart");
        }
        auto [_, chart] = *it;
        level = chart.level;
        resolution = static_cast<std::size_t>(chart.resolution);
        Toolkit::AffineTransform<float> memon_timing_to_300Hz(
            0.f, static_cast<float>(chart.resolution),
            -memon.offset*300.f, (-memon.offset+60.f/memon.BPM)*300.f
        );
        Toolkit::AffineTransform<float> memon_timing_to_300Hz_proportional(
            0.f, static_cast<float>(chart.resolution),
            0.f, (60.f/memon.BPM)*300.f
        );
        for (auto &&note : chart.notes) {
            auto timing = static_cast<long>(memon_timing_to_300Hz.transform(note.get_timing()));
            auto position = static_cast<Button>(note.get_pos());
            std::size_t length = 0;
            Button tail = Button::B1;
            if (note.get_length() != 0) {
                length = static_cast<std::size_t>(memon_timing_to_300Hz_proportional.transform(note.get_length()));
                tail = convert_memon_tail(position, note.get_tail_pos());
            }
            notes.insert({timing, position, length, tail});
        }
    }

    Button convert_memon_tail(Button note, unsigned int tail_position) {
        auto note_position = button_to_index(note);
        assert((note_position <= 15));
        assert((tail_position <= 11));
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

        auto tail_x = x+dx;
        auto tail_y = y+dy;
        if (tail_x < 0 or tail_x > 3 or tail_y < 0 or tail_y > 3) {
            throw std::runtime_error("Invalid tail_position : "+std::to_string(tail_position));
        }
        auto tail = coords_to_button({
            static_cast<unsigned int>(tail_x),
            static_cast<unsigned int>(tail_y)
        });
        if (not tail.has_value()) {
            throw std::runtime_error("Invalid tail_position : "+std::to_string(tail_position));
        }
        return *tail;
    }
}
