#pragma once

#include <jbcoe/polymorphic_value.h>
#include <SFML/Graphics.hpp>

#include "Panel.hpp"

namespace MusicSelect {

    class OptionPage;
    
    class SubpagePanel final : public Panel {
    public:
        SubpagePanel(
            SharedResources& resources,
            const jbcoe::polymorphic_value<OptionPage>& subpage,
            const std::string& name
        ) :
            Panel(resources),
            m_subpage(subpage),
            m_name(name)
        {};
        void click(Ribbon& ribbon, const Data::Button& button) override;
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        jbcoe::polymorphic_value<OptionPage> m_subpage;
        std::string m_name;
    };
}