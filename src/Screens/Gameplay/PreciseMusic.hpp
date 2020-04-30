#pragma once

#include <thread>
#include <atomic>

#include <SFML/Audio.hpp>
#include <SFML/System/Time.hpp>

namespace Gameplay {
    struct PreciseMusic : sf::Music {
        PreciseMusic();
        explicit PreciseMusic(std::string path);
        ~PreciseMusic();

        std::thread position_update_watchdog;

        void position_update_watchdog_main();
        std::atomic<bool> should_stop_watchdog = false;

        sf::Time getPrecisePlayingOffset() const;

        bool first_onGetData_call = true;
        sf::Clock lag_measurement;
        std::atomic<sf::Time> lag = sf::Time::Zero;
        std::atomic<bool> should_correct = false;
        std::atomic<sf::Time> last_music_position = sf::Time::Zero;
        sf::Clock last_position_update;
    protected:
        bool onGetData(sf::SoundStream::Chunk& data) override;
    };
}