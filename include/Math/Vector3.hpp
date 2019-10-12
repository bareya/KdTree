#ifndef MATH_VECTOR3_HPP
#define MATH_VECTOR3_HPP

#include "Types.hpp"

///
///
///
class Vector3
{
public:
    explicit Vector3(Real vv = 0.0_r)
        : xyz{vv, vv, vv}
    {
    }
    constexpr Vector3(Real xx, Real yy, Real zz)
        : xyz{xx, yy, zz}
    {
    }

    Vector3(const Vector3&) = default;
    Vector3(Vector3&&) = default;

    Vector3& operator=(const Vector3&) = default;
    Vector3& operator=(Vector3&&) = default;

    Vector3 operator-() const { return {-x(), -y(), -z()}; }

    Vector3 operator+(const Vector3& v) const { return {x() + v.x(), y() + v.y(), z() + v.z()}; }
    Vector3 operator-(const Vector3& v) const { return {x() - v.x(), y() - v.y(), z() - v.z()}; }
    Vector3 operator*(const Vector3& v) const { return {x() * v.x(), y() * v.y(), z() * v.z()}; }
    Vector3 operator/(const Vector3& v) const { return {x() / v.x(), y() / v.y(), z() / v.z()}; }

    Vector3 operator+(Real v) const { return {x() + v, y() + v, z() + v}; }
    Vector3 operator-(Real v) const { return {x() - v, y() - v, z() - v}; }
    Vector3 operator*(Real v) const { return {x() * v, y() * v, z() * v}; }
    Vector3 operator/(Real v) const { return {x() / v, y() / v, z() / v}; }

    Vector3& operator+=(const Vector3& v)
    {
        (*this) = (*this) + v;
        return *this;
    }
    Vector3& operator-=(const Vector3& v)
    {
        (*this) = (*this) - v;
        return *this;
    }
    Vector3& operator*=(const Vector3& v)
    {
        (*this) = (*this) * v;
        return *this;
    }
    Vector3& operator/=(const Vector3& v)
    {
        (*this) = (*this) / v;
        return *this;
    }

    Real& operator[](Index axis) { return xyz[axis]; }
    const Real& operator[](Index axis) const { return xyz[axis]; }

    Real& x() { return xyz[0]; }
    Real& y() { return xyz[1]; }
    Real& z() { return xyz[2]; }

    const Real& x() const { return xyz[0]; }
    const Real& y() const { return xyz[1]; }
    const Real& z() const { return xyz[2]; }

private:
    Real xyz[3];
};

inline Vector3 min(const Vector3& l, const Vector3 r)
{
    return {std::min(l.x(), r.x()), std::min(l.y(), r.y()), std::min(l.z(), r.z())};
}

inline Vector3 max(const Vector3& a, const Vector3& b)
{
    return {std::max(a.x(), b.x()), std::max(a.y(), b.y()), std::max(a.z(), b.z())};
}

#endif // MATH_VECTOR3_HPP
