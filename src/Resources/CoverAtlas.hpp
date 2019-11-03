#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

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
    
    /*
    A CoverAltas stores 256x256 cover previews in a vector of 1024x1024 textures
    */
    class CoverAltas {
    public:
        CoverAltas();
        // returns the sprite corresponding to the path
        // returns a fallback texture if the path is unknown
        sf::Sprite at(const fs::path& path) const;
        void emplace_back(const fs::path& cover);
        // only loads the images not already present in the atlas
        // empties the slots not present in the vector
        void efficient_reload(const std::vector<fs::path>& covers);
    
    private:
        std::unordered_map<fs::path, unsigned int> path_to_index;
        std::unordered_map<unsigned int, fs::path> index_to_path;
        std::vector<std::shared_ptr<sf::RenderTexture>> textures;
        unsigned int next_available_index();
    };

    struct DetailedLocation {
        unsigned int texture_index;
        unsigned int row;
        unsigned int column;
    };

    DetailedLocation get_detailed_location(unsigned int location);
}
