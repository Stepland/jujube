#pragma once

#include <memory>

#include <ghc/filesystem.hpp>
#include <SFML/Graphics.hpp>

#include "../Toolkit/Cache.hpp"

namespace fs = ghc::filesystem;

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

    // Hold time elapsed since loaded
    struct AutoloadedTexture {
        explicit AutoloadedTexture(std::shared_ptr<sf::Texture> t_texture) : texture(t_texture), loaded_since() {};
        std::shared_ptr<sf::Texture> texture;
        sf::Clock loaded_since;
    };

    AutoloadedTexture load_texture_from_path(const fs::path& path);
    
    using TextureCache = Toolkit::Cache<fs::path, AutoloadedTexture, &load_texture_from_path>;
}