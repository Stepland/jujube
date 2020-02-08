#pragma once

#include <filesystem>
#include <memory>
#include <optional>
#include <shared_mutex>
#include <string>
#include <unordered_map>
#include <unordered_set>

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
    // Loads textures asynchronously (in the background) on demand and stores them in a map for easy path-based access
    class Autoloader {
    public:
        Autoloader() = default;
        std::optional<std::shared_ptr<sf::Texture>> async_get(const fs::path& path);
        std::optional<std::shared_ptr<sf::Texture>> get(const fs::path& path);
        void load(const fs::path& path);
        void async_load(const fs::path& path);
        bool has(const fs::path& path);
        bool is_loading(const fs::path& path);
    private:
        std::unordered_map<fs::path, std::shared_ptr<sf::Texture>> m_mapping;
        std::shared_mutex m_mapping_mutex;
        std::unordered_set<fs::path> m_is_loading;
        std::shared_mutex m_is_loading_mutex;
    };
}