#pragma once

#include <SFML/System/Time.hpp>

#include "../../Data/Note.hpp"

namespace Gameplay {
    enum class Judgement {
        Perfect,
        Great,
        Good,
        Poor,
        Miss
    };

    Judgement delta_to_judgement(const sf::Time& delta);

    struct GradedNote : Data::Note {
        GradedNote(const Data::Note& n, const sf::Time& t) : Note::Note(n), delta(t), judgement(delta_to_judgement(t)) {};
        sf::Time delta;
        Judgement judgement;
    };
}