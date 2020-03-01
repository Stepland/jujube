#pragma once

#include <ghc/filesystem.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <SFML/Graphics.hpp>

#include "../Toolkit/GHCFilesystemPathHash.hpp"

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
        std::unordered_map<fs::path, std::size_t> path_to_index;
        std::unordered_map<std::size_t, fs::path> index_to_path;
        std::vector<std::shared_ptr<sf::RenderTexture>> textures;
        std::size_t next_available_index();
    };

    struct DetailedLocation {
        std::size_t texture_index;
        std::size_t row;
        std::size_t column;
    };

    DetailedLocation get_detailed_location(std::size_t location);
}
