#include "TextureCache.hpp"

#include <stdexcept>

namespace Textures {
    AutoloadedTexture load_texture_from_path(const fs::path& path) {
        auto texture = std::make_shared<sf::Texture>();
        if (!texture->loadFromFile(path.string())) {
            throw std::invalid_argument("Unable to load cover image : "+path.string());
        }
        texture->setSmooth(true);
        return AutoloadedTexture(texture);
    }
}
