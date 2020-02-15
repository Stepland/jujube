#pragma once

#include <optional>
#include <tuple>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "../../Data/SongList.hpp"
#include "../../Toolkit/AffineTransform.hpp"
#include "SharedResources.hpp"

namespace MusicSelect {

    class Ribbon;
    class SharedResources;

    // A Panel holds anything that can go under a button on the moving part
    // of the music select screen, be it nothing, a category indicator, or a song
    class Panel : public sf::Drawable, public sf::Transformable {
    public:
        Panel(SharedResources& resources) : m_resources(resources) {};
        // What happens when you click on the panel
        virtual void click(Ribbon& ribbon, std::size_t from_button_index) = 0;
        virtual ~Panel() = default;
    protected:
        SharedResources& m_resources;
        float get_size() const;
    };

    class EmptyPanel final : public Panel {
    public:
        using Panel::Panel;
        void click(Ribbon& ribbon, std::size_t from_button_index) override {return;};
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override {return;};
    };

    class ColoredMessagePanel final : public Panel {
    public:
        ColoredMessagePanel(SharedResources& resources, const sf::Color& color, const std::string& message) : Panel(resources), m_color(color), m_message(message) {};
        void click(Ribbon& ribbon, std::size_t from_button_index) override {return;};
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        const sf::Color m_color;
        const std::string m_message;
    };

    class ColorPanel final : public Panel {
    public:
        ColorPanel(SharedResources& resources, const sf::Color& t_color) : Panel(resources), m_color(t_color) {};
        void click(Ribbon& ribbon, std::size_t from_button_index) override {return;};
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        const sf::Color m_color;
    };

    class CategoryPanel final : public Panel {
    public:
        explicit CategoryPanel(SharedResources& resources, const std::string& t_label) : Panel(resources), m_label(t_label) {};
        void click(Ribbon& ribbon, std::size_t from_button_index) override;
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        std::string m_label;
    };

    struct ChartSelection {
        const Data::Song& song;
        const std::string& chart;
    };

    class SelectablePanel : public Panel {
    public:
        using Panel::Panel;
        virtual ~SelectablePanel() = default;
        virtual void unselect() = 0;
        virtual std::optional<ChartSelection> get_selected_chart() const = 0;
    };

    class SongPanel final : public SelectablePanel {
    public:
        explicit SongPanel(SharedResources& resources, const Data::Song& t_song) : SelectablePanel(resources), m_song(t_song) {};
        void click(Ribbon& ribbon, std::size_t from_button_index) override;
        void unselect() override;
        std::optional<ChartSelection> get_selected_chart() const override;
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        const Data::Song& m_song;
        const Toolkit::AffineTransform<float> m_seconds_to_alpha{0.0f, 0.15f, 0.f, 255.f};
        std::optional<std::string> selected_chart;
    };

    void set_to_global_bounds(sf::RectangleShape& rect, const sf::Text& text);
}