#include "GradedNote.hpp"

namespace Gameplay {
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
}
