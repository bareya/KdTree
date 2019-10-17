#ifndef MATH_BOUND_HPP
#define MATH_BOUND_HPP

#include "Types.hpp"

///
/// Bounded Arbitrary Interval
///
template <typename T> 
class BoundT
{
public:
    using value_type = T;

    BoundT() = default;

    BoundT(const T& mn, const T& mx)
        : min_(mn)
        , max_(mx)
    {
        //assert(min_ <= max_);
    }

    T Size() const { return max_ - min_; }
    T Center() const { return min_ + static_cast<T>(Size() * 0.5); }

    const T& Min() const { return min_; }
    const T& Max() const { return max_; }

    BoundT<T> Union(const T& rhs) const
    { 
        return BoundT{min(min_, rhs), max(max_, rhs)};
    }

    BoundT<T> Union(const BoundT<T>& rhs) const
    {
        return BoundT<T>{min(min_, rhs.Min()), max(max_, rhs.Max())};
    }

    std::pair<BoundT<T>, BoundT<T>> Split(Real ratio) const
    {
        assert(ratio >= 0 || ratio <= 1);

        T length = Size();
        T split = min_ + static_cast<T>(length * ratio);

        return std::make_pair(
            BoundT<T>{min_, split},
            BoundT<T>{split, max_});
    }

private:
    T min_;
    T max_;
};

#endif // MATH_BOUND_HPP
