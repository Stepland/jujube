#pragma once

#include <cstddef>

namespace Toolkit {
    struct DurationInFrames {
        std::size_t frames;
        std::size_t fps;
    };
}