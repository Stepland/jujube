#pragma once

#include <SFML/Audio/Music.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>

#include "AbstractMusic.hpp"

namespace Gameplay {
    // noop class for use when the chart has no associated music file
    struct Silence : AbstractMusic {
        Silence(const sf::Time& duration);
        void play() override;
        void pause() override;
        void stop() override;
        sf::SoundSource::Status getStatus() const override;
        sf::Time getPlayingOffset() const override;
        void setPlayingOffset(sf::Time timeOffset);
        sf::Time getDuration() const;
    private:
        const sf::Time m_duration;
        mutable sf::SoundSource::Status m_status;
        mutable sf::Time m_position;
        mutable sf::Clock m_clock;
    };
}