#include "GradedNote.hpp"

namespace Data {

    bool judgement_breaks_combo(Judgement j) {
        switch (j) {
            case Data::Judgement::Perfect:
            case Data::Judgement::Great:
            case Data::Judgement::Good:
                return false;
            default:
                return true;
        }
    }

    Resources::MarkerAnimation judgement_to_animation(Judgement j) {
        switch (j) {
            case Judgement::Perfect:
                return Resources::MarkerAnimation::PERFECT;
            case Judgement::Great:
                return Resources::MarkerAnimation::GREAT;
            case Judgement::Good:
                return Resources::MarkerAnimation::GOOD;
            case Judgement::Poor:
                return Resources::MarkerAnimation::POOR;
            case Judgement::Miss:
                return Resources::MarkerAnimation::MISS;
            default:
                return Resources::MarkerAnimation::APPROACH;
        }
    }

    Judgement delta_to_judgement(const sf::Time& delta) {
        auto delta_abs = delta;
        if (delta_abs < sf::Time::Zero) {
            delta_abs = -delta_abs;
        }
        if (delta_abs < sf::milliseconds(40)) {
            return Judgement::Perfect;
        } else if (delta_abs < sf::milliseconds(80)) {
            return Judgement::Great;
        } else if (delta_abs < sf::milliseconds(160)) {
            return Judgement::Good;
        } else if (delta_abs < sf::milliseconds(533)) {
            return Judgement::Poor;
        } else {
            return Judgement::Miss;
        }
    }
    // implement the strange way in which jubeat judges hold note releases 
    Judgement release_to_judgement(const sf::Time& duration_held, const sf::Time& note_duration, const int tail_length) {
        // if we implement hard mode we'll need a cleaner implementation of this since this would be 30. works for now though
        int error_margin = 60; 
        // take the length of the note in ticks on a 300 hz clock and divide by length of tail in pixels
        // logic taken from reversing the game's timing code
        double milliseconds_to_300hz = 0.3;
        auto note_duration_300hz = static_cast<double>(note_duration.asMilliseconds()) * milliseconds_to_300hz;
        auto adjusted_note_duration = static_cast<int>(
            note_duration_300hz * (1 - (static_cast<double>(error_margin) / (tail_length*195)))
        );

        // held for longer than duration : done
        if((duration_held.asMilliseconds() * milliseconds_to_300hz) >= adjusted_note_duration) {
            return Judgement::Perfect;
        }

        auto percentage_held = static_cast<int>(
            duration_held.asMilliseconds() * milliseconds_to_300hz / adjusted_note_duration * 100
        );

        if (percentage_held < 100 and percentage_held >= 81) {
            return Judgement::Great;
        } else if (percentage_held < 80 and percentage_held >= 51) {
            return Judgement::Good;
        } else {
            return Judgement::Poor;
        }
    }
}
