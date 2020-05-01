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
        // Here they are divided by 2 because we are checking against an absolute
        // offset from the target time position
        if (delta_abs < sf::milliseconds(21)) {
            return Judgement::Perfect;
        } else if (delta_abs < sf::milliseconds(46)) {
            return Judgement::Great;
        } else if (delta_abs < sf::milliseconds(83)) {
            return Judgement::Good;
        } else if (delta_abs < sf::milliseconds(250)) {
            return Judgement::Poor;
        } else {
            return Judgement::Miss;
        }
    }

    TimedJudgement JudgeFromDelta(const sf::Time& t) {
        auto res = TimedJudgement{};
        res.delta = t;
        res.judgement = delta_to_judgement(t);
        return res;
    }
}
