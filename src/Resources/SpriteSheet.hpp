#pragma once

#include <ghc/filesystem.hpp>
#include <nlohmann/json.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace fs = ghc::filesystem;

namespace Resources {
    struct DurationInFrames {
        std::size_t frames;
        std::size_t fps;
    };

    struct SpriteSheet {
        sf::Texture tex;
        fs::path tex_path;
        std::size_t count;
        std::size_t columns;
        std::size_t rows;

        void load_and_check(
            const fs::path& folder,
            std::size_t size,
            std::size_t fps,
            const DurationInFrames& max_duration
        );
    };

    void from_json(const nlohmann::json& j, SpriteSheet& s);
}
