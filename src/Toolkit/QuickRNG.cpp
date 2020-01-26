#include "QuickRNG.hpp"


Toolkit::UniformIntRNG::UniformIntRNG(int min, int max) : m_distribution(min, max) {
    std::random_device rd;
    m_generator = std::mt19937{rd()};
}

int Toolkit::UniformIntRNG::generate() {
    return m_distribution(m_generator);    
}
