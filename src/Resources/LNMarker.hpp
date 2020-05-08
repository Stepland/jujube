#pragma once

#include <ghc/filesystem.hpp>
#include <SFML/Graphics.hpp>

#include "SpriteSheet.hpp"

namespace fs = ghc::filesystem;

namespace Resources {
    struct LNMarker {
        LNMarker(const fs::path& folder);
        
        fs::path folder;
        std::string name;
        std::size_t fps;
        std::size_t size;
        SpriteSheet background;
        SpriteSheet outline;
        SpriteSheet highlight;
        SpriteSheet tail;
        SpriteSheet tip_appearance;
        SpriteSheet tip_begin_cycle;
        SpriteSheet tip_cycle;
    };

    void from_json(const nlohmann::json& j, LNMarker& m);
}