#pragma once

#include <mutex>
#include <thread>
#include <optional>
#include <shared_mutex>
#include <unordered_map>
#include <unordered_set>

namespace Toolkit {
    // Loads resources (audio, textures, etc ...) asynchronously in another thread and stores them for later reuse
    template <class Key, class Value, Value(*load_resource)(const Key&)>
    class Cache {
    public:
        Cache() = default;

        // Triggers async loading and returns empty if not already loaded
        std::optional<Value> async_get(const Key& key) {
            if (not has(key)) {
                if (not is_loading(key)) {
                    async_load(key);
                }
                return {};
            } else {
                return get(key);
            }
        }

        // Does not trigger loading
        std::optional<Value> get(const Key& key)  {
            std::shared_lock lock{m_mapping_mutex};
            if (has(key)) {
                return m_mapping.at(key);
            } else {
                return {};
            }
        }

        // Blocks until loaded
        Value blocking_get(const Key& key) {
            std::shared_lock lock{m_mapping_mutex};
            load(key);
            while (is_loading(key)) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            return m_mapping.at(key);
        }

        void load(const Key& key) {
            if (has(key) or is_loading(key)) {
                return;
            }
            {
                std::unique_lock lock{m_is_loading_mutex};
                m_is_loading.insert(key);
            }
            Value resource = load_resource(key);
            {
                std::unique_lock<std::shared_mutex> lock_mapping{m_mapping_mutex, std::defer_lock};
                std::unique_lock<std::shared_mutex> lock_is_loading{m_is_loading_mutex, std::defer_lock};
                std::lock(lock_mapping, lock_is_loading);
                m_mapping.emplace(key, resource);
                m_is_loading.erase(key);
            }
        }
        
        void async_load(const Key& key) {
            std::thread t(&Cache::load, this, key);
            t.detach();
        }

        bool has(const Key& key) {
            std::shared_lock lock{m_mapping_mutex};
            return m_mapping.find(key) != m_mapping.end();
        }

        bool is_loading(const Key& key) {
            std::shared_lock lock{m_is_loading_mutex};
            return m_is_loading.find(key) != m_is_loading.end();
        }

        void reserve(const std::size_t& n) {
            std::unique_lock<std::shared_mutex> lock_mapping{m_mapping_mutex, std::defer_lock};
            std::unique_lock<std::shared_mutex> lock_is_loading{m_is_loading_mutex, std::defer_lock};
            std::lock(lock_mapping, lock_is_loading);
            m_mapping.reserve(n);
            m_is_loading.reserve(n);
        }

    private:
        std::unordered_map<Key, Value> m_mapping;
        std::shared_mutex m_mapping_mutex;
        std::unordered_set<Key> m_is_loading;
        std::shared_mutex m_is_loading_mutex;
    };
}
