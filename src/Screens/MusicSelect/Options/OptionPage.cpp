#include "OptionPage.hpp"

#include <iostream>
#include <memory>
#include <vector>

#include "../Ribbon.hpp"
#include "../Panels/SubpagePanel.hpp"
#include "../Panels/MarkerPanel.hpp"
#include "InputRemap.hpp"

namespace MusicSelect {

    void OptionPage::update() {
        this->setPosition(get_ribbon_x(), get_ribbon_y());
    }

    RibbonPage::RibbonPage(const PanelLayout& layout, SharedResources& t_resources) :
        OptionPage(t_resources),
        m_ribbon(layout, t_resources),
        back_button(t_resources)
    {
    }

    bool MusicSelect::RibbonPage::handle_raw_input(const sf::Event::KeyEvent& key_event) {
        auto button = preferences.key_mapping.key_to_button(key_event.code);
        if (not button) {
            return false;
        }
        auto button_index = Data::button_to_index(*button);
        if (button_index > 13) {
            return false;
        }
        button_click(*button);
        return true;
    }

    void RibbonPage::button_click(const Data::Button& button) {
        resources.button_highlight.button_pressed(button);
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
        back_button.setPosition(2.f*get_panel_step(), 3.f*get_panel_step());
        target.draw(back_button, states);
    }

    MainOptionPage::MainOptionPage(SharedResources& t_resources) :
        RibbonPage(MainOptionPage::create_layout(t_resources), t_resources)
    {
    }

    PanelLayout MainOptionPage::create_layout(SharedResources& t_resources) {
        std::vector<std::shared_ptr<Panel>> subpages;
        auto marker_select = std::make_shared<MarkerSelect>(t_resources);
        subpages.emplace_back(std::make_shared<SubpagePanel>(t_resources, std::move(marker_select), "markers"));
        auto input_page = std::make_shared<InputOptionPage>(t_resources);
        subpages.emplace_back(std::make_shared<SubpagePanel>(t_resources, std::move(input_page), "input"));
        return PanelLayout{subpages, t_resources};
    }

    InputOptionPage::InputOptionPage(SharedResources& t_resources) :
        RibbonPage(InputOptionPage::create_layout(t_resources), t_resources)
    {
    }

    PanelLayout InputOptionPage::create_layout(SharedResources& t_resources) {
        std::vector<std::shared_ptr<Panel>> subpages;
        auto input_remap = std::make_shared<InputRemap>(t_resources);
        subpages.emplace_back(std::make_shared<SubpagePanel>(t_resources, std::move(input_remap), "remap\nbuttons"));
        return PanelLayout{subpages, t_resources};
    }

    MarkerSelect::MarkerSelect(SharedResources& t_resources) :
        RibbonPage(MarkerSelect::create_layout(t_resources), t_resources)
    {
    }

    MarkerSelect::~MarkerSelect() {
        resources.selected_marker.reset();
    }

    PanelLayout MarkerSelect::create_layout(SharedResources& t_resources) {
        std::vector<std::shared_ptr<Panel>> markers;
        for (const auto &[name, marker] : t_resources.markers) {
            markers.emplace_back(std::make_shared<MarkerPanel>(t_resources, marker));
        }
        return PanelLayout{markers, t_resources};
    }
}
