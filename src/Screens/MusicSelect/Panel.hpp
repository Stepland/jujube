#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "State.hpp"

namespace MusicSelect {
    // A Panel holds anything that can go under a panel on the moving part
    // of the music select screen, be it nothing, a sort indicator, or a song
    class Panel {
    public:
        // What happens when you click on the panel
        virtual void click(State& state) = 0;
        // How the panel should be displayed
        virtual void display(State& state, sf::Window& window, sf::FloatRect area) = 0;
        virtual ~Panel() = default;
    };

    class EmptyPanel final : public Panel {
    public:
        void click(State& state) override {return;};
        void display(State& state, sf::Window& window, sf::FloatRect area) override {return;};
    };

    class CategoryPanel final : public Panel {
    public:
        CategoryPanel(const std::string& t_label) : label(t_label) {};
        void click(State& state) override;
        void display(State& state, sf::Window& window, sf::FloatRect area) override;
    private:
        std::string label;
    };

    class SongPanel final : public Panel {
    public:
        SongPanel(const Data::Song& t_song) : song(t_song) {};
        void click(State& state) override;
        void display(State& state, sf::Window& window, sf::FloatRect area) override;
    private:
        const Data::Song& song;
    };
}