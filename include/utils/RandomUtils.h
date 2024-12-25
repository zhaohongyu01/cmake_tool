#pragma once

#include <cmath>
#include <cstdlib>
#include <vector>
#include <map>
#include <ctime>
#include <functional>
#include <random>

#include "UtilityCommon.hpp"


/**
 * @brief The RandomUtils class provides support for generating integer and floating point
 * random numbers.
 */
class CBTEK_UTILITY_DLL RandomUtils
{
public:
    /**
     * @brief Constructor that takes in optional seed
     * @param seed optional seed
     */
    RandomUtils(uint64_t seed=0xCB1022);

    /**
     * @brief reseed Reseeds the random number generator
     * @param seed Optional seed
     */
    void reseed(std::uint64_t seed = static_cast<std::uint64_t>(time(NULL)));


    /**
     * @brief random Returns random floating postd::int64_t value between 0 and 1
     * @return
     */
    double random(double min=0., double max=1.);

    /**
     * @brief Returns random integer between min and max(inclusive)
     * @param min The smallest random number to return
     * @param max The largest random number to return
     * @return
     */
    std::int64_t next(std::int64_t  min, std::int64_t max);

    /**
     * @brief next Returns random integer between 0 and max(inclusive)
     * @param max The largest random number to return
     * @return
     */
    std::int64_t next(std::int64_t  max);

private:    
    std::seed_seq m_seed;
    std::mt19937_64 m_rng;
};
