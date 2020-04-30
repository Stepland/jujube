#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

#include "Panel.hpp"

namespace MusicSelect {

    class OptionPage;
    
    class SubpagePanel final : public Panel {
    public:
        SubpagePanel(
            SharedResources& t_resources,
            std::shared_ptr<OptionPage> subpage,
            const std::string& name
        ) :
            Panel(t_resources),
            m_subpage(subpage),
            m_name(name)
        {};
        void click(Ribbon& ribbon, const Input::Button& button) override;
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        std::shared_ptr<OptionPage> m_subpage;
        std::string m_name;
    };
}