#pragma once

#include <optional>
#include <tuple>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "../../../Input/Buttons.hpp"
#include "../../../Data/Song.hpp"
#include "../../../Toolkit/AffineTransform.hpp"
#include "../DensityGraph.hpp"
#include "../SharedResources.hpp"

namespace MusicSelect {

    class Ribbon;

    // A Panel holds anything that can go under a button on the moving part
    // of the music select screen, be it nothing, a category indicator, or a song
    class Panel : public sf::Drawable, public sf::Transformable, public HoldsSharedResources {
    public:
        explicit Panel(SharedResources& t_resources);
        // What happens when you click on the panel
        virtual void click(Ribbon& ribbon, const Input::Button& button) = 0;
        virtual ~Panel() = default;
    protected:
        float get_size() const;
    };

    class EmptyPanel final : public Panel {
    public:
        using Panel::Panel;
        void click(Ribbon&, const Input::Button&) override {return;};
    private:
        void draw(sf::RenderTarget&, sf::RenderStates) const override {return;};
    };

    class ColoredMessagePanel final : public Panel {
    public:
        ColoredMessagePanel(SharedResources& t_resources, const sf::Color& color, const std::string& message) : Panel(t_resources), m_color(color), m_message(message) {};
        void click(Ribbon&, const Input::Button&) override {return;};
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        const sf::Color m_color;
        const std::string m_message;
    };

    class ColorPanel final : public Panel {
    public:
        ColorPanel(SharedResources& t_resources, const sf::Color& t_color) : Panel(t_resources), m_color(t_color) {};
        void click(Ribbon&, const Input::Button&) override {return;};
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        const sf::Color m_color;
    };

    class CategoryPanel final : public Panel {
    public:
        CategoryPanel(SharedResources& t_resources, const std::string& t_label) : Panel(t_resources), m_label(t_label) {};
        void click(Ribbon& ribbon, const Input::Button& button) override;
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        std::string m_label;
    };

    class SelectablePanel : public Panel {
    public:
        using Panel::Panel;
        virtual ~SelectablePanel() = default;
        virtual void unselect() = 0;
        virtual std::optional<Data::SongDifficulty> get_selected_difficulty() const = 0;
    };

    class SongPanel final : public SelectablePanel {
    public:
        explicit SongPanel(SharedResources& t_resources, const std::shared_ptr<const Data::Song>& t_song) : SelectablePanel(t_resources), m_song(t_song) {};
        void click(Ribbon& ribbon, const Input::Button& button) override;
        void unselect() override;
        std::optional<Data::SongDifficulty> get_selected_difficulty() const override;
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        std::shared_ptr<const Data::Song> m_song;
        const Toolkit::AffineTransform<float> m_seconds_to_alpha{0.0f, 0.15f, 0.f, 255.f};
        std::optional<std::string> selected_chart;
    };

    void set_to_global_bounds(sf::RectangleShape& rect, const sf::Text& text);
}