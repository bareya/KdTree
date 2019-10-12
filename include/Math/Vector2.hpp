#ifndef MATH_VECTOR2_HPP
#define MATH_VECTOR2_HPP

#include "Types.hpp"

class Vector2
{
public:
    explicit Vector2(Real vv = 0.0_r)
        : v{vv, vv}
    {
    }
    Vector2(Real xx, Real yy)
        : v{xx, yy}
    {
    }

    Vector2(const Vector2&) = default;
    Vector2(Vector2&&) = default;

    Vector2& operator=(const Vector2&) = default;
    Vector2& operator=(Vector2&&) = default;

    Vector2 operator-() const { return {-x(), -y()}; }

    Vector2 operator+(const Vector2& r) const { return {x() + r.x(), y() + r.y()}; }
    Vector2 operator-(const Vector2& r) const { return {x() - r.x(), y() - r.y()}; }
    Vector2 operator*(const Vector2& r) const { return {x() * r.x(), y() * r.y()}; }
    Vector2 operator/(const Vector2& r) const { return {x() / r.x(), y() / r.y()}; }

    Vector2 operator+(Real r) const { return {x() + r, y() + r}; }
    Vector2 operator-(Real r) const { return {x() - r, y() - r}; }
    Vector2 operator*(Real r) const { return {x() * r, y() * r}; }
    Vector2 operator/(Real r) const { return {x() / r, y() / r}; }

    Vector2& operator+=(const Vector2& r)
    {
        (*this) = (*this) + r;
        return *this;
    }
    Vector2& operator-=(const Vector2& r)
    {
        (*this) = (*this) - r;
        return *this;
    }
    Vector2& operator*=(const Vector2& r)
    {
        (*this) = (*this) * r;
        return *this;
    }
    Vector2& operator/=(const Vector2& r)
    {
        (*this) = (*this) / r;
        return *this;
    }

    Real& operator[](Index axis) { return v[axis]; }
    const Real& operator[](Index axis) const { return v[axis]; }

    Real& x() { return v[0]; }
    Real& y() { return v[1]; }

    const Real& x() const { return v[0]; }
    const Real& y() const { return v[1]; }

    Real Dot(const Vector2& r) const
    {
        return x()*r.x() + y()*r.y();
    }

    Real Magnitude2() const
    {
        return Dot(*this);
    }

private:
    Real v[2];
};

inline Vector2 min(const Vector2& l, const Vector2& r)
{
    return {std::min(l.x(), r.x()), std::min(l.y(), r.y())};
}

inline Vector2 max(const Vector2& a, const Vector2& b)
{
    return {std::max(a.x(), b.x()), std::max(a.y(), b.y())};
}

#endif // MATH_VECTOR2_HPP
