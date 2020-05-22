#pragma once

#include <stdexcept>

namespace Toolkit {
    template<typename T>
    class AffineTransform {
    public:
        AffineTransform(T low_input, T high_input, T low_output, T high_output):
            m_low_input(low_input),
            m_high_input(high_input),
            m_low_output(low_output),
            m_high_output(high_output)
            {
            if (low_input == high_input) {
                throw std::invalid_argument("low and high input values for affine transform must be different !");
            }
            m_a = (high_output-low_output)/(high_input-low_input);
            m_b = (high_input*low_output - high_output*low_input)/(high_input-low_input);
        };
        T transform(T val) const {return m_a*val + m_b;};
        T clampedTransform(T val) const { return transform(std::clamp(val,m_low_input,m_high_input));};
        T backwards_transform(T val) const {
            // if we're too close to zero
            if (std::abs(m_a) < 10e-10) {
                throw std::runtime_error("Can't apply backwards transformation, coefficient is too close to zero");
            } else {
                return (val-m_b)/m_a;
            }
        };
        void setB(T b) {
            m_b = b;
        }
    private:
        T m_a;
        T m_b;
        T m_low_input;
        T m_high_input;
        T m_low_output;
        T m_high_output;
    }; 
}
