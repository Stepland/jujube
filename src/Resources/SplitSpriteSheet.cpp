#include "SplitSpriteSheet.hpp"

#include <sstream>

#include <SFML/Graphics/RenderTexture.hpp>

namespace Resources {
    void from_json(const nlohmann::json& j, SplitSpriteSheet& s) {
        s.tex_path = fs::path{j.at("sprite_sheet").get<std::string>()};
        j.at("count").get_to(s.count);
        j.at("columns").get_to(s.columns);
        j.at("rows").get_to(s.rows);
    }

    void SplitSpriteSheet::load_and_check(
        const fs::path& folder,
        std::size_t size,
        std::size_t fps,
        const Toolkit::DurationInFrames& max_duration
    ) {
        // File Load & Check
        sf::Texture tex;
        if (not tex.loadFromFile(folder/tex_path)) {
            throw std::runtime_error(
                "Cannot open file "
                +(folder/tex_path).string()
            );
        }

        // Sprite sheet size check
        // throw if the texture size does not match what's announced by the metadata
        auto sheet_size = tex.getSize();
        auto expected_size = sf::Vector2u(columns, rows) * static_cast<unsigned int>(size);
        if (sheet_size != expected_size) {
            std::stringstream ss;
            ss << "Sprite sheet ";
            ss << (folder/tex_path).string();
            ss << " should be " << expected_size.x << "×" << expected_size.y << " pixels";
            ss << " but is " << sheet_size.x << "×" << sheet_size.y;
            throw std::invalid_argument(ss.str());
        }



        // Sprite count check
        // throw if the count calls for more sprites than possible according to the 'columns' and 'rows' fields
        if (count > columns * rows) {
            std::stringstream ss;
            ss << "Metadata for sprite sheet ";
            ss << (folder/tex_path).string();
            ss << " indicates that it holds " << count << " sprites";
            ss << " when it can only hold a maximum of " << columns * rows;
            ss << " according to the 'columns' and 'rows' fields";
            throw std::invalid_argument(ss.str());
        }

        // Duration check
        // We do not allow any marker animation to take longer than the jubeat standard of 16 frames at 30 fps
        // For that we make sure that :
        //     frames/fps <= max_frames/reference_fps
        // Which is mathematically equivalent to checking that :
        //     count*reference_fps <= max_frames*fps
        // Which allows us to avoid having to cast to float
        if (count*max_duration.fps > max_duration.frames*fps) {
            std::stringstream ss;
            ss << "Animation for sprite sheet ";
            ss << (folder/tex_path).string();
            ss << " lasts " << count/static_cast<float>(fps)*1000.f << "ms";
            ss << " (" << count << "f @ " << fps << "fps)";
            ss << " which is more than the maximum of ";
            ss << max_duration.frames/static_cast<float>(max_duration.fps)*1000.f << "ms";
            ss << " (16f @ 30fps)";
            throw std::invalid_argument(ss.str());
        }

        for (std::size_t frame = 0; frame < count; frame++) {
            sf::RenderTexture sprite_frame;
            sprite_frame.create(size, size);
            sprite_frame.setSmooth(true);
            sf::Sprite sprite{tex};
            sf::IntRect rect{
                sf::Vector2i{
                    static_cast<int>(frame % columns),
                    static_cast<int>(frame / columns)
                } * static_cast<int>(size),
                sf::Vector2i{
                    static_cast<int>(size),
                    static_cast<int>(size)
                }
            };
            sprite.setTextureRect(rect);
            sprite_frame.draw(sprite);
            textures.push_back(sprite_frame.getTexture());
            textures.back().setRepeated(true);
            textures.back().setSmooth(true);
        }
        
    }

    std::optional<sf::Sprite> SplitSpriteSheet::get_sprite(std::size_t frame) const {
        if (frame < count) {
            return sf::Sprite{textures.at(frame)};
        }
        return {};
    }
}
