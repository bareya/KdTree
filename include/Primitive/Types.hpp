#ifndef MATH_TYPES_HPP
#define MATH_TYPES_HPP

#include <limits>
#include <climits>
#include <cassert>
#include <vector>

//
// common types
//
using Real = float;
constexpr Real REAL_MAX = std::numeric_limits<Real>::max();
constexpr Real REAL_MIN = std::numeric_limits<Real>::min();
constexpr Real REAL_EPS = std::numeric_limits<Real>::epsilon();

using RealArray = std::vector<Real>;

using Index = long long;
constexpr Index INDEX_INVALID = static_cast<Index>(-1);

using IndexArray = std::vector<Index>;

//
// User defined literals
//
constexpr Real operator"" _r(long double val) { return static_cast<Real>(val); }
constexpr Real operator"" _r(unsigned long long val) { return static_cast<Real>(val); }
constexpr Index operator"" _i(unsigned long long val) { return static_cast<Index>(val); }

//
//
//
constexpr Index min(Index b, Index a) { return a < b ? a : b; }
constexpr Index max(Index b, Index a) { return a > b ? a : b; }

constexpr Real min(Real a, Real b) { return a < b ? a : b; }
constexpr Real max(Real a, Real b) { return a > b ? a : b; }

#endif // MATH_TYPES_HPP
