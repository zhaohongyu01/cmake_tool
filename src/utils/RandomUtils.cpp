#include "utils/RandomUtils.h"


RandomUtils::RandomUtils(std::uint64_t seed_value)
{
    reseed(seed_value);
}

void RandomUtils::reseed(std::uint64_t seed_value)
{    
    std::seed_seq seed{static_cast<std::uint64_t>(seed_value)};
    m_rng = std::mt19937_64(seed);
}

double RandomUtils::random(double min, double max)
{
    return std::uniform_real_distribution<double>(min,max)(m_rng);
}

std::int64_t RandomUtils::next(std::int64_t min, std::int64_t max)
{
    return std::uniform_int_distribution<std::int64_t>(min,max)(m_rng);
}

std::int64_t RandomUtils::next(std::int64_t max)
{
    return next(0,max);
}

