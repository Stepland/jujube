#pragma once

#include <string>
#include <map>

#include <ghc/filesystem.hpp>
#include <SFML/Graphics.hpp>

#include "SpriteSheet.hpp"
#include "SplitSpriteSheet.hpp"

namespace fs = ghc::filesystem;

namespace Resources {
    struct LNMarker {
        LNMarker(const fs::path& folder);

        std::optional<sf::Sprite> get_tail_sprite(sf::Time delta) const;

        std::optional<sf::Sprite> get_tip_sprite(sf::Time delta) const;
        std::optional<sf::Sprite> get_tip_appearance_sprite(int relative_frame) const;
        std::optional<sf::Sprite> get_tip_enter_cycle_sprite(int relative_frame) const;
        std::optional<sf::Sprite> get_tip_cycle_sprite(int relative_frame) const;

        std::optional<sf::Sprite> get_background_sprite(sf::Time delta) const;
        std::optional<sf::Sprite> get_outline_sprite(sf::Time delta) const;
        std::optional<sf::Sprite> get_highlight_sprite(sf::Time delta) const;

    
        fs::path folder;
        std::string name;
        std::size_t fps;
        std::size_t size;
        SpriteSheet background;
        SpriteSheet outline;
        SpriteSheet highlight;
        SplitSpriteSheet tail;
        SpriteSheet tip_appearance;
        SpriteSheet tip_enter_cycle;
        SpriteSheet tip_cycle;
    };

    void from_json(const nlohmann::json& j, LNMarker& m);

    class LNMarkers : public std::map<std::string, LNMarker> {
    public:
        LNMarkers(const fs::path& jujube_path);
    private:
        void load_from_folder(const fs::path& lnmarkers_folder);
    };
}