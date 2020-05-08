#pragma once

#include <string>
#include <map>

#include <ghc/filesystem.hpp>
#include <nlohmann/json.hpp>
#include <SFML/Graphics.hpp>

#include "SpriteSheet.hpp"

namespace fs = ghc::filesystem;

namespace Resources {
    enum class MarkerAnimation {
        APPROACH,
        MISS,
        POOR,
        GOOD,
        GREAT,
        PERFECT
    };

    struct Marker {
        explicit Marker(const fs::path& marker_folder);
        std::optional<sf::Sprite> get_sprite(const MarkerAnimation& state, const sf::Time seconds) const;
        std::optional<sf::Sprite> get_sprite(const MarkerAnimation& state, const float seconds) const;
        std::optional<sf::Sprite> get_sprite(const MarkerAnimation& state, const std::size_t frame) const;
        const SpriteSheet& get_sprite_sheet_from_enum(const MarkerAnimation& state) const;
        
        fs::path folder;
        std::string name;
        std::size_t size; // the side length in pixels
        std::size_t fps; // classic jubeat markers are 30 fps
        SpriteSheet approach;
        SpriteSheet miss;
        SpriteSheet poor;
        SpriteSheet good;
        SpriteSheet great;
        SpriteSheet perfect;
    };

    void from_json(const nlohmann::json& j, Marker& m);

    class Markers : public std::map<std::string, Marker> {
    public:
        Markers(const fs::path& jujube_path);
    private:
        void load_from_folder(const fs::path& markers_folder);
    };
}
