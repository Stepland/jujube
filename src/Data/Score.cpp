#include "Score.hpp"

#include <mutex>

#include <SFML/System/Time.hpp>

namespace Data {
    std::size_t count_classic_scoring_events(const std::set<Note>& notes) {
        std::size_t count = 0;
        for (auto&& note : notes) {
            if (note.duration > sf::Time::Zero) {
                count += 2;
            } else {
                count += 1;
            }
        }
        return count;
    }

    ClassicScore::ClassicScore(const std::set<Note>& notes) :
        tap_event_count(count_classic_scoring_events(notes)),
        judgement_counts(5)
    {
        auto denominator = static_cast<int>(std::min(static_cast<std::size_t>(1024), tap_event_count));
        shutter_incerment_2x = 2048/denominator;
        shutter_incerment_1x = 1024/denominator;
        shutter_decrement_4x = -8192/denominator;
        judgement_counts[Judgement::Perfect] = 0;
        judgement_counts[Judgement::Great] = 0;
        judgement_counts[Judgement::Good] = 0;
        judgement_counts[Judgement::Poor] = 0;
        judgement_counts[Judgement::Miss] = 0;
    }

    int ClassicScore::get_shutter() const {
        return shutter;
    }

    int ClassicScore::get_final_score() const {
        return get_score() + shutter*100000/1024;
    }

    int ClassicScore::get_score() const {
        return
            (
                90000 *
                (
                    judgement_counts.at(Judgement::Perfect) * 10 +
                    judgement_counts.at(Judgement::Great) * 7 +
                    judgement_counts.at(Judgement::Good) * 4 +
                    judgement_counts.at(Judgement::Poor) * 1
                )
            )
            / tap_event_count
        ;
    }

    Rating ClassicScore::get_rating() const {
        auto score = get_final_score();
        if (score < 500000) {
            return Rating::E;
        } else if (score < 700000) {
            return Rating::D;
        } else if (score < 800000) {
            return Rating::C;
        } else if (score < 850000) {
            return Rating::B;
        } else if (score < 900000) {
            return Rating::A;
        } else if (score < 950000) {
            return Rating::S;
        } else if (score < 980000) {
            return Rating::SS;
        } else if (score < 1000000) {
            return Rating::SSS;
        } else {
            return Rating::EXC;
        }
    }

    void ClassicScore::update(Judgement j) {
        judgement_counts[j] += 1;
        int shutter_delta = 0;
        switch (j) {
        case Judgement::Perfect:
        case Judgement::Great:
            shutter_delta = shutter_incerment_2x;
            break;
        case Judgement::Good:
            shutter_delta = shutter_incerment_1x;
            break;
        case Judgement::Poor:
        case Judgement::Miss:
            shutter_delta = shutter_decrement_4x;
            break;
        }
        shutter = std::clamp(shutter+shutter_delta, 0, 1024);
    }
}
