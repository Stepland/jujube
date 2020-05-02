#pragma once

#include <SFML/Audio/SoundSource.hpp>
#include <SFML/System/Time.hpp>

namespace Gameplay {
    // I'm rolling my own interface to allow for both Silence and
    // PreciseMusic to be used via an AbstractMusic pointer
    struct AbstractMusic {
        virtual ~AbstractMusic() = default;
        virtual void play() = 0;
        virtual void pause() = 0;
        virtual void stop() = 0;
        virtual sf::SoundSource::Status getStatus() const = 0;
        virtual sf::Time getPlayingOffset() const = 0;
    };
}