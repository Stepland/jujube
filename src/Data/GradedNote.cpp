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
}
