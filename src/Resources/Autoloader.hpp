#pragma once

#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

#include <SFML/Graphics.hpp>

namespace fs = std::filesystem;

// Define the way we hash fs::path for use in unordered maps
namespace std {
    template <>
    struct hash<fs::path> {
        std::size_t operator()(const fs::path& p) const {
            return std::hash<std::string>()(p.string());
        }
    };
}

namespace Textures {
    // Loads textures on demand and stores them in a map for easy path-based access
    class Autoloader {
    public:
        Autoloader() = default;
        std::shared_ptr<sf::Texture> get(const fs::path& t_path);
    private:
        void load(const fs::path& t_path);
        mutable std::unordered_map<fs::path, std::shared_ptr<sf::Texture>> m_mapping;
    }; 
}