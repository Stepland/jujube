#pragma once

#include <string>
#include <map>

#include <cereal/types/string.hpp>
#include <ghc/filesystem.hpp>
#include <SFML/Graphics.hpp>

namespace fs = ghc::filesystem;

template <class Archive>
std::string save_minimal(const Archive &, ghc::filesystem::path& p) {
    return p.string();
}

template <class Archive>
void load_minimal(const Archive &, ghc::filesystem::path& p, const std::string& value) {
    p = fs::path{value};
}

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

        template<class Archive>
        void serialize(Archive& archive) {
            archive(
                CEREAL_NVP(sprite_sheet),
                CEREAL_NVP(count),
                CEREAL_NVP(columns),
                CEREAL_NVP(rows)
            );
        }
    };

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

        template<class Archive>
        void serialize(Archive & archive) {
            archive(
                CEREAL_NVP(name),
                CEREAL_NVP(size),
                CEREAL_NVP(fps),
                CEREAL_NVP(approach),
                CEREAL_NVP(miss),
                CEREAL_NVP(early),
                CEREAL_NVP(good),
                CEREAL_NVP(great),
                CEREAL_NVP(perfect)
            );
        }
    };

    struct Marker {
        Marker(const fs::path& marker_folder);
        std::optional<sf::Sprite> get_sprite(const MarkerAnimation& state, const float& seconds);
        std::optional<sf::Sprite> get_sprite(const MarkerAnimation& state, const std::size_t frame);
        void load_and_check(sf::Texture& spritesheet, const MarkerAnimationMetadata& metadata);
        sf::Texture& get_sprite_sheet_from_enum(const MarkerAnimation& state);
        MarkerAnimationMetadata& get_metadata_from_enum(const MarkerAnimation& state);
        
        ghc::filesystem::path m_folder;
        MarkerMetadata m_metadata;
        sf::Texture m_approach;
        sf::Texture m_miss;
        sf::Texture m_early;
        sf::Texture m_good;
        sf::Texture m_great;
        sf::Texture m_perfect;
    };

    using Markers = std::multimap<std::string, Marker>;
    Markers load_markers();
}
