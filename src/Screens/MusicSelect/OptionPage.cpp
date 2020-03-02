#include "OptionPage.hpp"

#include <vector>

#include "Ribbon.hpp"
#include "Panels/SubpagePanel.hpp"
#include "Panels/MarkerPanel.hpp"

namespace MusicSelect {
    MainOptionPage::MainOptionPage(SharedResources& resources) :
        OptionPage(resources),
        Ribbon(MainOptionPage::create_layout(resources), resources)
    {
        
    }

    void MainOptionPage::click(const Data::Button& button) {
        click_on(button);
    }

    void MainOptionPage::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        this->Ribbon::draw(target, states);
    }

    PanelLayout MainOptionPage::create_layout(SharedResources& resources) {
        std::vector<jbcoe::polymorphic_value<Panel>> subpages;
        jbcoe::polymorphic_value<OptionPage> marker_select{MarkerSelect{resources}};
        subpages.emplace_back(SubpagePanel{resources, marker_select, "markers"});
        return PanelLayout{subpages, resources};
    }

    MarkerSelect::MarkerSelect(SharedResources& resources) :
        OptionPage(resources),
        Ribbon(MarkerSelect::create_layout(resources), resources)
    {

    }

    void MarkerSelect::click(const Data::Button& button) {
        click_on(button);
    }

    void MarkerSelect::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        this->Ribbon::draw(target, states);
    }

    PanelLayout MarkerSelect::create_layout(SharedResources& resources) {
        std::vector<jbcoe::polymorphic_value<Panel>> markers;
        for (const auto &[name, marker] : resources.markers) {
            markers.emplace_back(MarkerPanel{resources, marker});
        }
        return PanelLayout{markers, resources};
    }
    
}
