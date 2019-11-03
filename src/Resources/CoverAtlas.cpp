#include "CoverAtlas.hpp"

#include <algorithm>
#include <cassert>
#include <sstream>

namespace fs = std::filesystem;

Textures::CoverAltas::CoverAltas() :
    path_to_index(),
    index_to_path(),
    textures()
{
    this->emplace_back("assets/textures/fallback_cover.png");
}

sf::Sprite Textures::CoverAltas::at(const fs::path& path) const {
    
    unsigned int index;
    if (path_to_index.find(path) != path_to_index.end()) {
        index = path_to_index.at(path);
    } else {
        index = 0;
    }

    auto location = get_detailed_location(index);
    sf::IntRect rect(location.column*256, location.row*256, 256, 256);
    sf::Sprite cover;
    cover.setTexture(textures.at(location.texture_index)->getTexture());
    cover.setTextureRect(rect);
    return cover;
}

void Textures::CoverAltas::emplace_back(const fs::path& cover) {

    if (path_to_index.find(cover) != path_to_index.end()) {
        return;
    }

    sf::Texture cover_texture;
    if (!cover_texture.loadFromFile(cover)) {
        throw std::invalid_argument("Unable to load cover image : "+cover.string());
    }
    unsigned int next_index = next_available_index();
    auto size = cover_texture.getSize();
    auto location = get_detailed_location(next_index);
    sf::Sprite new_cover;
    new_cover.setTexture(cover_texture);
    new_cover.setScale(256.0f/size.x, 256.0f/size.y);
    new_cover.setPosition(256.0f * location.column, 256.0f * location.row);
    if (location.column == 0 and location.row == 0) {
        // first cover means it's a new texture
        textures.push_back(std::make_shared<sf::RenderTexture>());
        textures.back()->create(1024, 1024);
    }
    textures.at(location.texture_index)->draw(new_cover);
    textures.at(location.texture_index)->display();
    path_to_index[cover] = next_index;
    index_to_path[next_index] = cover;
}

void Textures::CoverAltas::efficient_reload(const std::vector<fs::path>& covers) {
    
    // Remove the paths that are not present in covers
    auto it = path_to_index.begin();
    while (it != path_to_index.end()) {
        if (std::find(covers.begin(), covers.end(), it->first) == covers.end()) {
            index_to_path.erase(it->second);
            it = path_to_index.erase(it);
        } else {
            ++it;
        }
    }

    // Add the covers that are not already in the altas
    for (const auto& cover : covers) {
        if (path_to_index.find(cover) == path_to_index.end()) {
            this->emplace_back(cover);
        }
    }
}

Textures::DetailedLocation Textures::get_detailed_location(unsigned int location) {
    return {location / 16, (location % 16) / 4, location % 4};
}

unsigned int Textures::CoverAltas::next_available_index() {
    unsigned int index = 0;
    while (index_to_path.find(index) != index_to_path.end()) {
        ++index;
    }
    return index;
}
