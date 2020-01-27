#include "ButtonHighlight.hpp"

MusicSelect::ButtonHighlight::ButtonHighlight(std::size_t t_panel_size) :
    panel_size(t_panel_size),
    highlight({static_cast<float>(t_panel_size-3), static_cast<float>(t_panel_size-3)}),
    time_to_alpha(0.f, 0.25f, 255.f, 0.f)
{
    highlight.setFillColor(sf::Color::Transparent);
    highlight.setOutlineThickness(1.f);
    highlight.setOrigin(highlight.getSize().x / 2.f, highlight.getSize().y / 2.f);
}

void MusicSelect::ButtonHighlight::button_pressed(Button button) {
    button_presses_history[button].restart();
}

void MusicSelect::ButtonHighlight::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    auto it = button_presses_history.begin();
	while (it != button_presses_history.end()) {
        auto elapsed = it->second.getElapsedTime();
        auto coords = toCoords(it->first);
		if (elapsed > sf::milliseconds(250)) {
			it = button_presses_history.erase(it);
		} else {
			auto alpha = time_to_alpha.transform(elapsed.asSeconds());
            highlight.setOutlineColor(sf::Color(255,255,0,static_cast<std::size_t>(alpha)));
            highlight.setPosition({
                static_cast<float>(coords.x * panel_size) + panel_size/2.f,
                static_cast<float>(coords.y * panel_size) + panel_size/2.f
            });
            target.draw(highlight, states);
            ++it;
		}
	}
}
