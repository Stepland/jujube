#pragma once

#include "../../Resources/SharedResources.hpp"

namespace Gameplay {
    struct ScreenResources : Resources::HoldsSharedResources {
        ScreenResources(Resources::SharedResources& r) : Resources::HoldsSharedResources(r) {};
    };

    struct HoldsResources : Resources::HoldsSharedResources {
        HoldsResources(ScreenResources& t_resources) : Resources::HoldsSharedResources(t_resources.shared), resources(t_resources) {};
        ScreenResources& resources;
    };
}