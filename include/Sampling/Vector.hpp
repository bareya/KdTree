#ifndef RANDOM_VECTOR_H
#define RANDOM_VECTOR_H

#include <Linear/Vector.hpp>

#include <random>
#include <vector>

inline void create_random_samples(std::vector<Vector2>& vectors, size_t size, 
    const Vector2& mn = {0, 0}, const Vector2& mx = {1, 1})
{
    vectors.reserve(size);

    std::mt19937 mt{0};
    std::uniform_real_distribution<Real> dist_x{mn.x(), mx.x()};
    std::uniform_real_distribution<Real> dist_y{mn.y(), mx.y()};

    for (size_t i{}; i < size; ++i)
    {
        vectors.emplace_back(dist_x(mt), dist_y(mt));
    }
}

#endif // RANDOM_VECTOR_H
