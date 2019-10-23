#include "JacketPack.hpp"

#include <algorithm>
#include <cassert>
#include <sstream>

sf::Sprite Textures::JacketPack::operator[](const std::string& path) const{
    auto location = get_detailed_location(locations.at(path));
    sf::IntRect rect(location.column, location.row, 128, 128);
    sf::Sprite jacket;
    jacket.setTexture(textures.at(location.texture_index)->getTexture());
    jacket.setTextureRect(rect);
    return jacket;
}

void Textures::JacketPack::push_back(const std::filesystem::path& jacket) {

    sf::Texture jacket_texture;
    if (!jacket_texture.loadFromFile(jacket)) {
        std::stringstream ss;
        ss << "Unable to load jacket image : " << jacket;
        throw std::runtime_error(ss.str());
    } else {
        locations[jacket] = next_location;
    }
    auto size = jacket_texture.getSize();
    auto location = get_detailed_location(next_location);
    sf::Sprite new_jacket;
    new_jacket.setTexture(jacket_texture);
    new_jacket.setScale(128.0f/size.x, 128.0f/size.y);
    new_jacket.setPosition(128.0f * location.column, 128.0f * location.row);
    if (location.column == 0 and location.row == 0) {
        // first jacket means it's a new texture
        textures.push_back(std::make_shared<sf::RenderTexture>());
        textures.back()->create(1024, 1024);
    }
    textures.at(location.texture_index)->draw(new_jacket);
    textures.at(location.texture_index)->display();
    next_location++;
}

Textures::DetailedLocation Textures::get_detailed_location(unsigned int location) {
    return {location / 64, (location % 64) / 8, location % 8};    
}
