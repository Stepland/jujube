#pragma once

#include <SFML/Audio/Music.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>

namespace Gameplay {
    // noop class for use when the chart has no associated music file
    class Silence : public sf::Music {
    public:
        Silence(const sf::Time& duration);
        void play();
        void pause();
        void stop();
        unsigned int getChannelCount() const;
        unsigned int getSampleRate() const;
        sf::SoundSource::Status getStatus() const;
        void setPlayingOffset(sf::Time timeOffset);
        sf::Time getPlayingOffset() const;
        void setLoop(bool /*loop*/) {};
        bool getLoop() const {return false;};
        bool openFromFile(const std::string& /*filename*/) {return false;};
        bool openFromMemory(const void* /*data*/, std::size_t /*sizeInBytes*/) {return false;};
        bool openFromStream(sf::InputStream& /*stream*/) {return false;};
        sf::Time getDuration() const;
        TimeSpan getLoopPoints() const {return TimeSpan{};};
        void setLoopPoints(TimeSpan /*timePoints*/) {};
    private:
        const sf::Time m_duration;
        mutable sf::SoundSource::Status m_status;
        mutable sf::Time m_position;
        mutable sf::Clock m_clock;
    };
}