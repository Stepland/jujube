#include "Autoloader.hpp"

#include <chrono>
#include <thread>

void Textures::Autoloader::load(const fs::path& t_path) {
    if (m_mapping.find(t_path) != m_mapping.end()) {
        return;
    }
    auto texture = std::make_shared<sf::Texture>();
    if (!texture->loadFromFile(t_path)) {
        throw std::invalid_argument("Unable to load cover image : "+t_path.string());
    }
    texture->setSmooth(true);
    m_mapping[t_path] = texture;
}

std::shared_ptr<sf::Texture> Textures::Autoloader::get(const fs::path& t_path) {
    if (m_mapping.find(t_path) == m_mapping.end()) {
        this->load(t_path);
    }
    return m_mapping.at(t_path);
}

bool Textures::Autoloader::has(const fs::path& t_path) {
    return m_mapping.find(t_path) != m_mapping.end();
}
