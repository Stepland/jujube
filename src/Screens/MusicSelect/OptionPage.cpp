#include "OptionPage.hpp"

#include <iostream>
#include <memory>
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
    }


    PanelLayout MainOptionPage::create_layout(SharedResources& resources) {
        std::vector<std::shared_ptr<Panel>> subpages;
        auto marker_select = std::make_shared<MarkerSelect>(MarkerSelect{resources});
        subpages.emplace_back(std::make_shared<SubpagePanel>(resources, marker_select, "markers"));
        return PanelLayout{subpages, resources};
    }

    MarkerSelect::MarkerSelect(SharedResources& resources) :
        RibbonPage(MarkerSelect::create_layout(resources), resources)
    {
    }

    MarkerSelect::~MarkerSelect() {
        m_resources.selected_marker.reset();
    }

    PanelLayout MarkerSelect::create_layout(SharedResources& resources) {
        std::vector<std::shared_ptr<Panel>> markers;
        for (const auto &[name, marker] : resources.markers) {
            markers.emplace_back(std::make_shared<MarkerPanel>(resources, marker));
        }
        return PanelLayout{markers, resources};
    }
}
