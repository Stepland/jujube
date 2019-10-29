#include "CoverPack.hpp"

#include <algorithm>
#include <cassert>
#include <sstream>

sf::Sprite Textures::CoverPack::at(const std::filesystem::path& path) const{
    auto location = get_detailed_location(locations.at(path));
    sf::IntRect rect(location.column, location.row, 128, 128);
    sf::Sprite cover;
    cover.setTexture(textures.at(location.texture_index)->getTexture());
    cover.setTextureRect(rect);
    return cover;
}

void Textures::CoverPack::emplace_back(const std::filesystem::path& cover) {

    sf::Texture cover_texture;
    if (!cover_texture.loadFromFile(cover)) {
        std::stringstream ss;
        ss << "Unable to load cover image : " << cover;
        throw std::invalid_argument(ss.str());
    } else {
        locations[cover] = next_location;
    }
    auto size = cover_texture.getSize();
    auto location = get_detailed_location(next_location);
    sf::Sprite new_cover;
    new_cover.setTexture(cover_texture);
    new_cover.setScale(128.0f/size.x, 128.0f/size.y);
    new_cover.setPosition(128.0f * location.column, 128.0f * location.row);
    if (location.column == 0 and location.row == 0) {
        // first cover means it's a new texture
        textures.push_back(std::make_shared<sf::RenderTexture>());
        textures.back()->create(1024, 1024);
    }
    textures.at(location.texture_index)->draw(new_cover);
    textures.at(location.texture_index)->display();
    next_location++;
}

Textures::DetailedLocation Textures::get_detailed_location(unsigned int location) {
    return {location / 64, (location % 64) / 8, location % 8};    
}

const std::vector<std::shared_ptr<sf::RenderTexture>>& Textures::CoverPack::getTextures() const {
    return textures;    
}
