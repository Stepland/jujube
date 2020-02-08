#include "Autoloader.hpp"

#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

void Textures::Autoloader::load(const fs::path& path) {
    if (has(path) or is_loading(path)) {
        return;
    }
    {
        std::unique_lock lock{m_is_loading_mutex};
        m_is_loading.insert(path);
    }
    auto texture = std::make_shared<sf::Texture>();
    if (!texture->loadFromFile(path)) {
        throw std::invalid_argument("Unable to load cover image : "+path.string());
    }
    texture->setSmooth(true);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    {
        std::unique_lock<std::shared_mutex> lock_mapping{m_mapping_mutex, std::defer_lock};
        std::unique_lock<std::shared_mutex> lock_is_loading{m_is_loading_mutex, std::defer_lock};
        std::lock(lock_mapping, lock_is_loading);
        m_mapping.emplace(path, texture);
        m_is_loading.erase(path);
    }
    std::cout << "Loaded " << path << std::endl;
}

void Textures::Autoloader::async_load(const fs::path& path) {
    std::thread t(&Textures::Autoloader::load, this, path);
    t.detach();
}

std::optional<std::shared_ptr<sf::Texture>> Textures::Autoloader::async_get(const fs::path& path) {
    if (not has(path)) {
        if (not is_loading(path)) {
            async_load(path);
        }
        return {};
    } else {
        return get(path);
    }
}

std::optional<std::shared_ptr<sf::Texture>> Textures::Autoloader::get(const fs::path& path) {
    std::shared_lock lock{m_mapping_mutex};
    if (has(path)) {
        return m_mapping.at(path);
    } else {
        return {};
    }
}


bool Textures::Autoloader::has(const fs::path& path) {
    std::shared_lock lock{m_mapping_mutex};
    return m_mapping.find(path) != m_mapping.end();
}

bool Textures::Autoloader::is_loading(const fs::path& path) {
    std::shared_lock lock{m_is_loading_mutex};
    return m_is_loading.find(path) != m_is_loading.end();
}
