#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "../../Data/SongList.hpp"
#include "Resources.hpp"

namespace MusicSelect {

    class Screen;

    // A Panel holds anything that can go under a button on the moving part
    // of the music select screen, be it nothing, a sort indicator, or a song
    class Panel {
    public:
        // What happens when you click on the panel
        virtual void click(Screen& screen) = 0;
        // How the panel should be displayed
        virtual void draw(Resources& resources, sf::RenderTarget& target, sf::FloatRect area) = 0;
        virtual ~Panel() = default;
    };

    class EmptyPanel final : public Panel {
    public:
        void click(Screen& screen) override {return;};
        void draw(Resources& resources, sf::RenderTarget& target, sf::FloatRect area) override {return;};
    };

    class CategoryPanel final : public Panel {
    public:
        explicit CategoryPanel(const std::string& t_label) : label(t_label) {};
        void click(Screen& screen) override;
        void draw(Resources& resources, sf::RenderTarget& target, sf::FloatRect area) override;
    private:
        std::string label;
    };

    class SongPanel final : public Panel {
    public:
        explicit SongPanel(const Data::Song& t_song) : song(t_song) {};
        void click(Screen& screen) override;
        void draw(Resources& resources, sf::RenderTarget& target, sf::FloatRect area) override;
    private:
        const Data::Song& song;
    };
}