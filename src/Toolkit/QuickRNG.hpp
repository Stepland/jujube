#pragma once

#include <random>

namespace Toolkit {
    class UniformIntRNG {
    public:
        UniformIntRNG(int min, int max);
        int generate();
    private:
        std::mt19937 m_generator;
        std::uniform_int_distribution<> m_distribution;
    };
}