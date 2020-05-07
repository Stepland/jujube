#pragma once

#include <mutex>
#include <optional>
#include <queue>

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

namespace Gameplay {
    struct TimedEvent {
        sf::Time time;
        sf::Event event;
    };

    // Thread-safe queue for events with a timestamp
    class TimedEventsQueue {
    public:
        TimedEventsQueue() = default;
        void push(const TimedEvent& te);
        std::optional<TimedEvent> pop();
    private:
        std::queue<TimedEvent> m_queue;
        std::mutex m_mutex;
    };
}
