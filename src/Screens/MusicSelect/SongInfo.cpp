#include "SongInfo.hpp"

#include <SFML/Graphics.hpp>

namespace MusicSelect {

    SongInfo::SongInfo(SharedResources& resources, const float& width, const float& height) :
        m_resources(resources),
        m_width(width),
        m_height(height),
        m_cover_fallback({height*0.8f, height*0.8f})
    {
        m_cover_fallback.setFillColor(sf::Color::Black);
        m_cover_fallback.setOutlineThickness(1.f);
        m_cover_fallback.setOutlineColor(sf::Color::White);
        m_cover_fallback.setPosition(height*0.1f, height*0.1f);
    }

    void SongInfo::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        draw_cover(target, states);
    }
}

void MusicSelect::SongInfo::draw_cover(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(m_cover_fallback, states);
    auto selected_panel = m_resources.selected_panel;
    if (not selected_panel.has_value()) {
        return;
    }
    auto selected_chart = selected_panel->panel.get_selected_chart();
    if (not selected_chart.has_value()) {
        return;
    }
    auto cover_path = selected_chart->song.full_cover_path();
    if (not cover_path.has_value()) {
        return;
    }
    auto cover_texture = m_resources.covers.async_get(*cover_path);
    if (not cover_texture.has_value()) {
        return;
    }
    sf::Sprite cover{*(cover_texture->texture)};
    auto bounds = cover.getGlobalBounds();
    cover.setScale(m_height*0.8f/bounds.width, m_height*0.8f/bounds.height);
    cover.setPosition(m_height*0.1f, m_height*0.1f);
    auto alpha = static_cast<std::uint8_t>(
        m_seconds_to_alpha.clampedTransform(
            selected_panel->selected_since.getElapsedTime().asSeconds()
        )
    );
    cover.setColor(sf::Color(255, 255, 255, alpha));
    target.draw(cover, states);
}
