#pragma once

#include <optional>

#include <SFML/System/Time.hpp>

#include "../Resources/Marker.hpp"
#include "Note.hpp"

namespace Data {
    enum class Judgement {
        Perfect,
        Great,
        Good,
        Poor,
        Miss,
    };

    Resources::MarkerAnimation judgement_to_animation(const Judgement& j);

    Judgement delta_to_judgement(const sf::Time& delta);

    struct TimedJudgement {
        TimedJudgement() : delta(sf::Time::Zero), judgement(Judgement::Miss) {};
        TimedJudgement(const sf::Time& d, const Judgement& j) : delta(d), judgement(j) {};
        explicit TimedJudgement(const sf::Time& t) : delta(t), judgement(delta_to_judgement(t)) {};
        sf::Time delta;
        Judgement judgement;
    };

    struct GradedNote : Data::Note {
        GradedNote() = default;
        GradedNote(const Data::Note& n) : Note::Note(n) {};
        GradedNote(const Data::Note& n, const sf::Time& t) : Note::Note(n), timed_judgment(t) {};
        std::optional<TimedJudgement> timed_judgment;
    };
}