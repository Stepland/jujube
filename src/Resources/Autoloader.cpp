#include "Autoloader.hpp"

#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

namespace Textures {
    void Autoloader::load(const fs::path& path) {
        if (has(path) or is_loading(path)) {
            return;
        }
        {
            std::unique_lock lock{m_is_loading_mutex};
            m_is_loading.insert(path);
        }
        auto texture = std::make_shared<sf::Texture>();
        if (!texture->loadFromFile(path.string())) {
            throw std::invalid_argument("Unable to load cover image : "+path.string());
        }
        texture->setSmooth(true);
        {
            std::unique_lock<std::shared_mutex> lock_mapping{m_mapping_mutex, std::defer_lock};
            std::unique_lock<std::shared_mutex> lock_is_loading{m_is_loading_mutex, std::defer_lock};
            std::lock(lock_mapping, lock_is_loading);
            m_mapping.emplace(path, texture);
            m_is_loading.erase(path);
        }
    }

    void Autoloader::async_load(const fs::path& path) {
        std::thread t(&Autoloader::load, this, path);
        t.detach();
    }

    std::optional<AutoloadedTexture> Autoloader::async_get(const fs::path& path) {
        if (not has(path)) {
            if (not is_loading(path)) {
                async_load(path);
            }
            return {};
        } else {
            return get(path);
        }
    }

    std::optional<AutoloadedTexture> Autoloader::get(const fs::path& path) {
        std::shared_lock lock{m_mapping_mutex};
        if (has(path)) {
            return m_mapping.at(path);
        } else {
            return {};
        }
    }


    bool Autoloader::has(const fs::path& path) {
        std::shared_lock lock{m_mapping_mutex};
        return m_mapping.find(path) != m_mapping.end();
    }

    bool Autoloader::is_loading(const fs::path& path) {
        std::shared_lock lock{m_is_loading_mutex};
        return m_is_loading.find(path) != m_is_loading.end();
    }
}