#pragma once

#include <thread>
#include <atomic>

#include <SFML/Audio/Music.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

#include "AbstractMusic.hpp"

namespace Gameplay {
    struct _PreciseMusic : sf::Music {
        explicit _PreciseMusic(const std::string& path);
        ~_PreciseMusic();

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

    struct PreciseMusic : AbstractMusic {
        explicit PreciseMusic(const std::string& path) : m_precise_music(path) {};
        void play() override {m_precise_music.play();};
        void pause() override {m_precise_music.pause();};
        void stop() override {m_precise_music.stop();};
        sf::SoundSource::Status getStatus() const override {return m_precise_music.getStatus();};
        sf::Time getPlayingOffset() const override {return m_precise_music.getPrecisePlayingOffset();};
    private:
        _PreciseMusic m_precise_music;
    };
}