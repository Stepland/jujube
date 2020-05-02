#include "GradedNote.hpp"

namespace Gameplay {
    Resources::MarkerAnimation judgement_to_animation(const Judgement& j) {
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
        // Numbers from http://544332133981.hatenablog.com/entry/bemani-rank_4
        // which themselves are apparently from jubeat analyser's code.
        if (delta_abs < sf::milliseconds(42)) {
            return Judgement::Perfect;
        } else if (delta_abs < sf::milliseconds(92)) {
            return Judgement::Great;
        } else if (delta_abs < sf::milliseconds(166)) {
            return Judgement::Good;
        } else {
            return Judgement::Poor;
        }
    }

    TimedJudgement JudgeFromDelta(const sf::Time& t) {
        auto res = TimedJudgement{};
        res.delta = t;
        res.judgement = delta_to_judgement(t);
        return res;
    }
}
