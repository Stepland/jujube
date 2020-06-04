#pragma once

#include <set>
#include <shared_mutex>
#include <string>
#include <variant>
#include <unordered_map>

#include "Note.hpp"
#include "GradedNote.hpp"

// Forward declarations
namespace Gameplay {
    class Screen;
}

namespace Data {
    enum class Rating {
        EXC,
        SSS,
        SS,
        S,
        A,
        B,
        C,
        D,
        E
    };

    const std::unordered_map<Rating, std::string> rating_to_string = {
        {Rating::EXC, "EXC"},
        {Rating::SSS, "SSS"},
        {Rating::SS, "SS"},
        {Rating::S, "S"},
        {Rating::A, "A"},
        {Rating::B, "B"},
        {Rating::C, "C"},
        {Rating::D, "D"},
        {Rating::E, "E"}
    };

    // Abstract interface for scoring systems
    class AbstractScore {
    public:
        virtual ~AbstractScore() = default;
        // Return final score (normal score + shutter bonus)
        virtual int get_final_score() const = 0;
        // Get current score (i.e. without shutter bonus)
        virtual int get_score() const = 0;
        // Return the current rating
        virtual Rating get_rating() const = 0;
        // Update score according to the recieved judgement
        virtual void update(Judgement j) = 0;
        // Get judgments
        virtual int get_judgement_counts(Judgement j) const = 0;
    };

    std::size_t count_classic_scoring_events(const std::set<Note>& notes);

    // Classic jubeat scoring
    class ClassicScore final : public AbstractScore {
    public:
        ClassicScore(const std::set<Note>& notes);
        int get_shutter() const;
        int get_final_score() const override;
        int get_score() const override;
        Rating get_rating() const override;
        void update(Judgement j) override;
        int get_judgement_counts(Judgement j) const;
    private:
        int shutter = 0;
        int shutter_incerment_2x;
        int shutter_incerment_1x;
        int shutter_decrement_4x;
        const std::size_t tap_event_count;
        std::unordered_map<Judgement, std::size_t> judgement_counts;
        friend class Gameplay::Screen;
    };
}