#pragma once

#include <memory>

#include <ghc/filesystem.hpp>
#include <SFML/Graphics.hpp>

#include "../Toolkit/Cache.hpp"
#include "../Toolkit/GHCFilesystemPathHash.hpp"

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