#include "Silence.hpp"

namespace Gameplay {
    Silence::Silence(const sf::Time& duration) :
        m_duration(duration),
        m_status(sf::Music::Stopped)
    {
    }

    void Silence::play() {
        if (m_status != sf::SoundSource::Paused) {
            m_position = sf::Time::Zero;
        }
        m_clock.restart();
    }

    void Silence::pause() {
        if (m_status == sf::SoundSource::Playing) {
            m_position += m_clock.getElapsedTime();
            if (m_position > m_duration) {
                stop();
            } else {
                m_status = sf::Music::Paused;
            }
        }
    }

    void Silence::stop() {
        m_status = sf::Music::Stopped;
        m_position = sf::Time::Zero;
    }

    unsigned int getChannelCount() {
        return 2;
    }

    unsigned int getSampleRate() {
        return 48000;
    }

    sf::SoundSource::Status Silence::getStatus() const {
        return m_status;
    }

    void Silence::setPlayingOffset(sf::Time timeOffset) {
        if (timeOffset > m_duration) {
            stop();
        } else {
            m_position = timeOffset;
        }
    }

    sf::Time Silence::getPlayingOffset() const {
        if (m_status == sf::SoundSource::Playing) {
            m_position += m_clock.restart();
            if (m_position > m_duration) {
                m_status = sf::Music::Stopped;
                m_position = sf::Time::Zero;
            }
        }
        return m_position;
    }

    sf::Time Silence::getDuration() const {
        return m_duration;
    }
}