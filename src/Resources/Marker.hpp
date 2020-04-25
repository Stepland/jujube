#pragma once

#include <string>
#include <map>

#include <ghc/filesystem.hpp>
#include <nlohmann/json.hpp>
#include <SFML/Graphics.hpp>

namespace Resources {
    enum class MarkerAnimation {
        APPROACH,
        MISS,
        EARLY, // or LATE
        GOOD,
        GREAT,
        PERFECT
    };

    struct MarkerAnimationMetadata {
        ghc::filesystem::path sprite_sheet;
        std::size_t count;  // how many sprites total on the sheet
        std::size_t columns;  // how many horizontally
        std::size_t rows;  // how many vertically
    };

    void to_json(nlohmann::json& j, const MarkerAnimationMetadata& mam);
    void from_json(const nlohmann::json& j, MarkerAnimationMetadata& mam);

    // Represents what's held in marker.json
    struct MarkerMetadata {
        std::string name;
        std::size_t size; // the side length in pixels
        std::size_t fps; // classic jubeat markers are 30 fps
        MarkerAnimationMetadata approach;
        MarkerAnimationMetadata miss;
        MarkerAnimationMetadata early;
        MarkerAnimationMetadata good;
        MarkerAnimationMetadata great;
        MarkerAnimationMetadata perfect;
    };

    void to_json(nlohmann::json& j, const MarkerMetadata& mm);
    void from_json(const nlohmann::json& j, MarkerMetadata& mm);

    struct Marker {
        explicit Marker(const ghc::filesystem::path& marker_folder);
        std::optional<sf::Sprite> get_sprite(const MarkerAnimation& state, float seconds) const;
        std::optional<sf::Sprite> get_sprite(const MarkerAnimation& state, const std::size_t frame) const;
        void load_and_check(sf::Texture& spritesheet, const MarkerAnimationMetadata& metadata);
        const sf::Texture& get_sprite_sheet_from_enum(const MarkerAnimation& state) const;
        const MarkerAnimationMetadata& get_metadata_from_enum(const MarkerAnimation& state) const;
        
        ghc::filesystem::path m_folder;
        MarkerMetadata m_metadata;
        sf::Texture m_approach;
        sf::Texture m_miss;
        sf::Texture m_early;
        sf::Texture m_good;
        sf::Texture m_great;
        sf::Texture m_perfect;
    };

    using Markers = std::map<std::string, Marker>;
    Markers load_markers(const ghc::filesystem::path& jujube_path);
}
