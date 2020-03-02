#include "OptionPage.hpp"

#include <vector>

#include "Ribbon.hpp"
#include "Panels/SubpagePanel.hpp"

namespace MusicSelect {
    MainOptionPage::MainOptionPage(SharedResources& resources) :
        OptionPage(resources),
        Ribbon(MainOptionPage::create_layout(resources), resources)
    {
        
    }

    void MainOptionPage::click(const Data::Button& button) {
        click_on(button);
    }

    PanelLayout MainOptionPage::create_layout(SharedResources& resources) {
        std::vector<jbcoe::polymorphic_value<Panel>> subpages;
        jbcoe::polymorphic_value<OptionPage> marker_select{MarkerSelect{resources}};
        subpages.emplace_back(SubpagePanel{resources, marker_select, "markers"});
        return PanelLayout{subpages, resources};
    }

    
}
