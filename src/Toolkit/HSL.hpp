// Taken from https://github.com/SFML/SFML/wiki/Source%3A-HSL-Color
#ifndef HSL_COLOR
#define HSL_COLOR

#include <SFML/Graphics/Color.hpp>
#include <algorithm>
#include <cmath>

namespace Toolkit {
    struct HSL {
    public:
        double Hue;
        double Saturation;
        double Luminance;

        HSL();
        HSL(int H, int S, int L);

        sf::Color TurnToRGB();

    private:
        double HueToRGB(double arg1, double arg2, double H);
    };

    HSL TurnToHSL(const sf::Color& C);
}

#endif // HSL_COLOR