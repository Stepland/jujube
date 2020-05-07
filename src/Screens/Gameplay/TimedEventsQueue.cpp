#include "TimedEventsQueue.hpp"

namespace Gameplay {
    void TimedEventsQueue::push(const TimedEvent& te) {
        std::unique_lock lock{m_mutex};
        m_queue.push(te);
    }

    std::optional<TimedEvent> TimedEventsQueue::pop() {
        std::unique_lock lock{m_mutex};
        if (m_queue.empty()) {
            return {};
        } else {
            TimedEvent res = m_queue.front();
            m_queue.pop();
            return res;
        }
    }
}
