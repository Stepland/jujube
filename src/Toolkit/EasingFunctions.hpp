#pragma once

#include <cmath>

namespace Toolkit {
    class EaseExponential {
    public:
        EaseExponential(float t_easing_factor) : easing_factor(t_easing_factor) {};
        float transform(float in);
        float easing_factor;
    };
}