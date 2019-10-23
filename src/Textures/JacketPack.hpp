#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <SFML/Graphics.hpp>

namespace Textures {
    
    /*
    A JacketPack stores 128x128 jacket previews in 1024x1024 textures
    */
    class JacketPack {
    public:
        JacketPack() = default;
        sf::Sprite operator[](const std::string& path) const;
        void push_back(const std::filesystem::path& jacket);
    //private:
        std::unordered_map<std::filesystem::path, unsigned int> locations;
        std::vector<std::shared_ptr<sf::RenderTexture>> textures;
        unsigned int next_location = 0;
    };

    struct DetailedLocation {
        unsigned int texture_index;
        unsigned int row;
        unsigned int column;
    };

    DetailedLocation get_detailed_location(unsigned int location);
}
