#include "EasingFunctions.hpp"

float Toolkit::EaseExponential::transform(float in) {
    // (1-2^(a*x))/(1-2^a)
    return (
        1.f - std::pow(2.f, easing_factor*in)
    ) / (
        1.f - std::pow(2.f, easing_factor)
    );
}
