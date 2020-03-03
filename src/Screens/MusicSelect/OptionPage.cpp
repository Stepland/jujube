#include "OptionPage.hpp"

#include <vector>

#include "Ribbon.hpp"
#include "Panels/SubpagePanel.hpp"
#include "Panels/MarkerPanel.hpp"

namespace MusicSelect {

    void OptionPage::update() {
        this->setPosition(get_ribbon_x(), get_ribbon_y());
    }

    RibbonPage::RibbonPage(const PanelLayout& layout, SharedResources& resources) :
        OptionPage(resources),
        m_ribbon(layout, resources)
    {
        update();
    }

    void RibbonPage::click(const Data::Button& button) {
        auto button_index = Data::button_to_index(button);
        if (button_index < 12) {
            m_ribbon.click_on(button);
        } else {
            switch (button) {
            case Data::Button::B13:
                m_ribbon.move_left();
                break;
            case Data::Button::B14:
                m_ribbon.move_right();
                break;
            default:
                break;
            }
        }
    }

    void RibbonPage::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        target.draw(m_ribbon, states);
    }

    MainOptionPage::MainOptionPage(SharedResources& resources) :
        RibbonPage(MainOptionPage::create_layout(resources), resources)
    {
        update();
    }


    PanelLayout MainOptionPage::create_layout(SharedResources& resources) {
        std::vector<jbcoe::polymorphic_value<Panel>> subpages;
        jbcoe::polymorphic_value<OptionPage> marker_select{MarkerSelect{resources}};
        subpages.emplace_back(SubpagePanel{resources, marker_select, "markers"});
        return PanelLayout{subpages, resources};
    }

    MarkerSelect::MarkerSelect(SharedResources& resources) :
        RibbonPage(MarkerSelect::create_layout(resources), resources)
    {
        update();
    }

    PanelLayout MarkerSelect::create_layout(SharedResources& resources) {
        std::vector<jbcoe::polymorphic_value<Panel>> markers;
        for (const auto &[name, marker] : resources.markers) {
            markers.emplace_back(MarkerPanel{resources, marker});
        }
        return PanelLayout{markers, resources};
    }
}
