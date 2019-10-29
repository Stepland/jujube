#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <SFML/Graphics.hpp>

// Define the way we hash std::filesystem::path for use in unordered maps
namespace std {
    template <>
    struct hash<std::filesystem::path> {
        std::size_t operator()(const std::filesystem::path& p) const {
            return std::hash<std::string>()(p.string());
        }
    };
};

namespace Textures {
    
    /*
    A CoverPack stores 128x128 cover previews in a vector of 1024x1024 textures
    */
    class CoverPack {
    public:
        CoverPack() = default;
        sf::Sprite at(const std::filesystem::path& path) const;
        void emplace_back(const std::filesystem::path& cover);
        const std::vector<std::shared_ptr<sf::RenderTexture>>& getTextures() const;
    private:
        std::unordered_map<std::filesystem::path, unsigned int> locations;
        std::vector<std::shared_ptr<sf::RenderTexture>> textures;
        unsigned int next_location = 0;
    };

    struct DetailedLocation {
        unsigned int texture_index;
        unsigned int row;
        unsigned int column;
    };

    DetailedLocation get_detailed_location(unsigned int location);
};
