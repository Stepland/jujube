#include "Marker.hpp"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>

#include <cereal/types/string.hpp>
#include <cereal/archives/json.hpp>

namespace fs = ghc::filesystem;

namespace Resources {

    Marker::Marker(const fs::path& marker_folder) :
        m_folder(marker_folder),
        m_metadata(),
        m_approach(),
        m_miss(),
        m_early(),
        m_good(),
        m_great(),
        m_perfect()
    {
        if (not fs::is_directory(m_folder)) {
            throw std::invalid_argument(m_folder.string()+" is not a folder");
        }
        if (not fs::exists(m_folder/"marker.json")) {
            throw std::invalid_argument("Marker folder ( "+m_folder.string()+" ) has no marker.json file");
        }
        std::ifstream marker_json{m_folder/"marker.json"};
        {
            cereal::JSONInputArchive archive{marker_json};
            m_metadata.serialize(archive);
        }
        load_and_check(m_approach, m_metadata.approach);
        load_and_check(m_miss, m_metadata.miss);
        load_and_check(m_early, m_metadata.early);
        load_and_check(m_good, m_metadata.good);
        load_and_check(m_great, m_metadata.great);
        load_and_check(m_perfect, m_metadata.perfect);
    }

    void Marker::load_and_check(sf::Texture& sprite_sheet, const MarkerAnimationMetadata& metadata) {
        // File Load & Check
        if (not sprite_sheet.loadFromFile(m_folder/metadata.sprite_sheet)) {
            throw std::runtime_error(
                "Cannot open marker sprite sheet "
                +(m_folder/metadata.sprite_sheet).string()
            );
        }
        
        // Sprite sheet size check
        // throw if the texture size does not match what's announced by the metadata
        auto sheet_size = sprite_sheet.getSize();
        auto expected_size = sf::Vector2u(metadata.columns, metadata.rows) * static_cast<unsigned int>(m_metadata.size);
        if (sheet_size != expected_size) {
            std::stringstream ss;
            ss << "Marker sprite sheet ";
            ss << (m_folder/metadata.sprite_sheet).string();
            ss << " should be " << expected_size.x << "×" << expected_size.y << " pixels";
            ss << " but is " << sheet_size.x << "×" << sheet_size.y;
            throw std::invalid_argument(ss.str());
        }

        // Sprite count check
        // throw if the count calls for more sprites than possible according to the 'columns' and 'rows' fields
        if (metadata.count > metadata.columns * metadata.rows) {
            std::stringstream ss;
            ss << "Metadata for marker sprite sheet ";
            ss << (m_folder/metadata.sprite_sheet).string();
            ss << " indicates that it holds " << metadata.count << " sprites";
            ss << " when it can only hold a maximum of " << metadata.columns * metadata.rows;
            ss << " according to the 'columns' and 'rows' fields";
            throw std::invalid_argument(ss.str());
        }

        // Duration check
        // We do not allow any marker animation to take longer than the jubeat standard of 16 frames at 30 fps
        // For that we make sure that :
        //     count/fps <= 16/30
        // Which is mathematically equivalent to checking that :
        //     count*30 <= 16*fps
        // Which allows us to avoid having to cast to float
        if (metadata.count*30 > 16*m_metadata.fps) {
            std::stringstream ss;
            ss << "Marker animation for sprite sheet ";
            ss << (m_folder/metadata.sprite_sheet).string();
            ss << " lasts " << metadata.count/static_cast<float>(m_metadata.fps)*1000.f << "ms";
            ss << " (" << metadata.count << "f @ " << m_metadata.fps << "fps)";
            ss << " which is more than the maximum of " << 16.f/30.f*1000.f << "ms";
            ss << " (16f @ 30fps)";
            throw std::invalid_argument(ss.str());
        }
    }

    const sf::Texture& Marker::get_sprite_sheet_from_enum(const MarkerAnimation& state) const {
        switch (state) {
        case MarkerAnimation::APPROACH:
            return m_approach;
            break;
        case MarkerAnimation::MISS:
            return m_miss;
            break;
        case MarkerAnimation::EARLY:
            return m_early;
            break;
        case MarkerAnimation::GOOD:
            return m_good;
            break;
        case MarkerAnimation::GREAT:
            return m_great;
            break;
        case MarkerAnimation::PERFECT:
            return m_perfect;
            break;
        default:
            throw std::runtime_error("wtf ?");
        }
    }

    const MarkerAnimationMetadata& Marker::get_metadata_from_enum(const MarkerAnimation& state) const {
        switch (state) {
        case MarkerAnimation::APPROACH:
            return m_metadata.approach;
            break;
        case MarkerAnimation::MISS:
            return m_metadata.miss;
            break;
        case MarkerAnimation::EARLY:
            return m_metadata.early;
            break;
        case MarkerAnimation::GOOD:
            return m_metadata.good;
            break;
        case MarkerAnimation::GREAT:
            return m_metadata.great;
            break;
        case MarkerAnimation::PERFECT:
            return m_metadata.perfect;
            break;
        default:
            throw std::runtime_error("wtf ?");
        }
    }

    std::optional<sf::Sprite> Marker::get_sprite(const MarkerAnimation& state, float seconds) const {
        auto raw_frame = static_cast<int>(std::floor(seconds*m_metadata.fps));
        if (raw_frame >= 0) {
            if (state == MarkerAnimation::APPROACH) {
                return get_sprite(MarkerAnimation::MISS, static_cast<std::size_t>(raw_frame));
            } else {
                return get_sprite(state, static_cast<std::size_t>(raw_frame));
            }
        } else {
            auto approach_frame_count = get_metadata_from_enum(MarkerAnimation::APPROACH).count;
            return get_sprite(
                MarkerAnimation::APPROACH,
                static_cast<std::size_t>(raw_frame+static_cast<int>(approach_frame_count))
            );
        }
    }

    std::optional<sf::Sprite> Marker::get_sprite(const MarkerAnimation& state, const std::size_t frame) const {
        auto& meta = get_metadata_from_enum(state);
        if (frame >= meta.count) {
            return {};
        } else {
            auto& tex = get_sprite_sheet_from_enum(state);
            sf::Sprite sprite{tex};
            sf::IntRect rect{
                sf::Vector2i{
                    static_cast<int>(frame % meta.columns),
                    static_cast<int>(frame / meta.columns)
                } * static_cast<int>(m_metadata.size),
                sf::Vector2i{
                    static_cast<int>(m_metadata.size),
                    static_cast<int>(m_metadata.size)
                }
            };
            sprite.setTextureRect(rect);
            return sprite;
        }
    }

    Markers load_markers() {
        Markers res;
        if (fs::exists(fs::path("markers"))) {
            for (auto& p : fs::directory_iterator("markers")) {
                if (p.is_directory()) {
                    try {
                        Marker m{p.path()};
                        res.emplace(m.m_metadata.name, m);
                    } catch (const std::exception& e) {
                        std::cerr << "Unable to load marker folder "
                        << p.path().filename().string() << " : "
                        << e.what() << std::endl;
                    }
                }
            }
        }
        if (res.empty()) {
            throw std::runtime_error("No markers found in marker folder, jujube needs at least one to operate");
        }
        return res;
    }
}
