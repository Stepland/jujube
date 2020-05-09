#include "LNMarker.hpp"

#include <fstream>
#include <iostream>

namespace Resources {
    LNMarker::LNMarker(const fs::path& t_folder) :
        folder(t_folder)
    {
        if (not fs::is_directory(folder)) {
            throw std::invalid_argument(folder.string()+" is not a folder");
        }
        if (not fs::exists(folder/"long.json")) {
            throw std::invalid_argument("LNMarker folder ( "+folder.string()+" ) has no long.json file");
        }
        std::ifstream marker_json{folder/"long.json"};
        nlohmann::json j;
        marker_json >> j;
        j.get_to(*this);
        background.load_and_check(folder, size, fps, {16, 30});
        outline.load_and_check(folder, size, fps, {16, 30});
        highlight.load_and_check(folder, size, fps, {16, 30});
        tail.load_and_check(folder, size, fps, {16, 30});
        tip_appearance.load_and_check(folder, size, fps, {16, 30});
        tip_enter_cycle.load_and_check(folder, size, fps, {8, 30});
        tip_cycle.load_and_check(folder, size, fps, {16, 30});
    }

    std::optional<sf::Sprite> LNMarker::get_tail_sprite(sf::Time delta) const {
        if (delta >= sf::seconds(-16.f/30.f)) {
            auto raw_frame = static_cast<int>(std::floor(delta.asSeconds()*fps));
            while (raw_frame <= 0) {
                raw_frame += static_cast<int>(tail.count);
            }
            return tail.get_sprite(static_cast<std::size_t>(raw_frame % tail.count));
        }
        return {};
    }

    std::optional<sf::Sprite> LNMarker::get_tip_sprite(sf::Time delta) const {
        if (delta >= sf::seconds(-16.f/30.f)) {
            auto relative_frame = static_cast<int>(std::floor(delta.asSeconds()*fps));
            if (delta < sf::Time::Zero) {
                return get_tip_appearance_sprite(relative_frame);
            } else if (relative_frame < static_cast<int>(tip_enter_cycle.count)) {
                return get_tip_enter_cycle_sprite(relative_frame);
            } else {
                return get_tip_cycle_sprite(relative_frame);
            }
        }
        return {};
    }

    std::optional<sf::Sprite> LNMarker::get_tip_appearance_sprite(int relative_frame) const {
        if (relative_frame < 0) {
            auto frame_relative_to_start = static_cast<int>(tip_appearance.count) + relative_frame;
            if (frame_relative_to_start > 0) {
                return tip_appearance.get_sprite(
                    static_cast<std::size_t>(frame_relative_to_start),
                    size
                );
            }
        }
        return {};
    }

    std::optional<sf::Sprite> LNMarker::get_tip_enter_cycle_sprite(int relative_frame) const {
        if (relative_frame >= 0 and relative_frame < static_cast<int>(tip_enter_cycle.count)) {
            return tip_enter_cycle.get_sprite(static_cast<std::size_t>(relative_frame), size);
        }
        return {};
    }

    std::optional<sf::Sprite> LNMarker::get_tip_cycle_sprite(int relative_frame) const {
        if (relative_frame >= static_cast<int>(tip_enter_cycle.count)) {
            return tip_cycle.get_sprite(
                static_cast<std::size_t>((relative_frame-tip_enter_cycle.count)%tip_cycle.count),
                size
            );
        }
        return {};
    }

    std::optional<sf::Sprite> LNMarker::get_background_sprite(sf::Time delta) const {
        if (delta >= sf::seconds(-16.f/30.f)) {
            auto raw_frame = static_cast<int>(std::floor(delta.asSeconds()*fps));
            while (raw_frame <= 0) {
                raw_frame += static_cast<int>(background.count);
            }
            return background.get_sprite(static_cast<std::size_t>(raw_frame%background.count), size);
        }
        return {};
    }

    std::optional<sf::Sprite> LNMarker::get_outline_sprite(sf::Time delta) const {
        if (delta >= sf::seconds(-16.f/30.f)) {
            auto raw_frame = static_cast<int>(std::floor(delta.asSeconds()*fps));
            while (raw_frame <= 0) {
                raw_frame += static_cast<int>(outline.count);
            }
            return outline.get_sprite(static_cast<std::size_t>(raw_frame%outline.count), size);
        }
        return {};
    }

    std::optional<sf::Sprite> LNMarker::get_highlight_sprite(sf::Time delta) const {
        if (delta >= sf::seconds(-16.f/30.f)) {
            auto raw_frame = static_cast<int>(std::floor(delta.asSeconds()*fps));
            while (raw_frame <= 0) {
                raw_frame += static_cast<int>(highlight.count);
            }
            return highlight.get_sprite(static_cast<std::size_t>(raw_frame%highlight.count), size);
        }
        return {};
    }

    void from_json(const nlohmann::json& j, LNMarker& m) {
        j.at("name").get_to(m.name);
        j.at("fps").get_to(m.fps);
        j.at("size").get_to(m.size);
        j.at("note").at("background").get_to(m.background);
        j.at("note").at("outline").get_to(m.outline);
        j.at("note").at("highlight").get_to(m.highlight);
        j.at("tail").get_to(m.tail);
        j.at("tip").at("appearance").get_to(m.tip_appearance);
        j.at("tip").at("enter cycle").get_to(m.tip_enter_cycle);
        j.at("tip").at("cycle").get_to(m.tip_cycle);
    }

    LNMarkers::LNMarkers(const fs::path& jujube_path) {
        load_from_folder(jujube_path/"markers"/"long");
        load_from_folder(jujube_path/"assets"/"markers"/"long");
        if (empty()) {
            throw std::runtime_error("No long note markers found, jujube needs at least one to operate");
        }
    }
    void LNMarkers::load_from_folder(const fs::path& lnmarkers_folder) {
        if (fs::exists(lnmarkers_folder)) {
            for (auto& p : fs::directory_iterator(lnmarkers_folder)) {
                if (p.is_directory()) {
                    try {
                        LNMarker m{p.path()};
                        emplace(m.name, m);
                    } catch (const std::exception& e) {
                        std::cerr << "Unable to load long note marker folder ";
                        std::cerr << "'" << p.path().filename().string() << "' : "
                        << e.what() << '\n';
                    }
                }
            }
        }
    }
}
