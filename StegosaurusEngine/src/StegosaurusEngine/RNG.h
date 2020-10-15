#pragma once

#include <StegosaurusEngine/Core.h>

#include <random>

namespace Steg {

    struct RNG {

    private:

        std::default_random_engine generator;

        std::uniform_int_distribution<uint32_t> rand;

    public:

        RNG() = delete;

        RNG(uint32_t seed, uint32_t upperBound);

        RNG(uint32_t seed);

        uint32_t Next();

    };

}
