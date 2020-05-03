#pragma once

#include <set>
#include <shared_mutex>
#include <variant>
#include <unordered_map>

#include "Note.hpp"
#include "GradedNote.hpp"

// Forward declarations
namespace Gameplay {
    class Screen;
}

namespace Data {
    // Thread-safe abstract interface for scoring systems
    class AbstractScore {
    public:
        virtual ~AbstractScore() = default;
        // Return final score (normal score + shutter bonus)
        int get_final_score() const;
        // Get current score (i.e. without shutter bonus)
        int get_score() const;
        // Update score according to the recieved judgement
        void update(Judgement j);
    protected:
        // non thread-safe version of the public interface
        virtual int unsafe_get_final_score() const = 0;
        virtual int unsafe_get_score() const = 0;
        virtual void unsafe_update(Judgement j) = 0;
        mutable std::shared_mutex mutex;
    };

    std::size_t count_classic_scoring_events(const std::set<Note>& notes);

    // Classic jubeat scoring
    class ClassicScore final : public AbstractScore {
    public:
        ClassicScore(const std::set<Note>& notes);
        int get_shutter() const;
    private:
        int unsafe_get_final_score() const override;
        int unsafe_get_score() const override;
        void unsafe_update(Judgement j) override;

        int shutter = 0;
        int shutter_incerment_2x;
        int shutter_incerment_1x;
        int shutter_decrement_4x;
        const std::size_t tap_event_count;
        std::unordered_map<Judgement, std::size_t> judgement_counts;

        friend class Gameplay::Screen;
    };
}