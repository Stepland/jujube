#include "Score.hpp"

#include <mutex>

#include <SFML/System/Time.hpp>

namespace Data {
    int AbstractScore::get_final_score() const {
        std::shared_lock lock{mutex};
        return unsafe_get_final_score();
    }

    int AbstractScore::get_score() const {
        std::shared_lock lock{mutex};
        return unsafe_get_score();
    }

    void AbstractScore::update(Judgement j) {
        std::unique_lock lock{mutex};
        unsafe_update(j);
    }

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
        std::shared_lock lock{mutex};
        return shutter;
    }

    int ClassicScore::unsafe_get_final_score() const {
        return unsafe_get_score() + shutter*100000/1024;
    }

    int ClassicScore::unsafe_get_score() const {
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

    void ClassicScore::unsafe_update(Judgement j) {
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
