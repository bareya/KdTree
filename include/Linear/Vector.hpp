#ifndef LINEAR_VECTOR_HPP
#define LINEAR_VECTOR_HPP

#include <Primitive/Types.hpp>

///
///
///
class Vector2
{
public:
    constexpr static Index DIM = 2;

    explicit Vector2(Real vv = 0.0_r)
        : xyz{vv, vv}
    {
    }
    constexpr Vector2(Real xx, Real yy)
        : xyz{xx, yy}
    {
    }

    Vector2(const Vector2&) = default;
    Vector2(Vector2&&) = default;

    Vector2& operator=(const Vector2&) = default;
    Vector2& operator=(Vector2&&) = default;

    Vector2 operator-() const { return {-x(), -y()}; }
    Vector2 operator+(const Vector2& v) const { return {x() + v.x(), y() + v.y()}; }
    Vector2 operator-(const Vector2& v) const { return {x() - v.x(), y() - v.y()}; }
    Vector2 operator*(const Vector2& v) const { return {x() * v.x(), y() * v.y()}; }
    Vector2 operator/(const Vector2& v) const { return {x() / v.x(), y() / v.y()}; }
    Vector2 operator+(Real v) const { return {x() + v, y() + v}; }
    Vector2 operator-(Real v) const { return {x() - v, y() - v}; }
    Vector2 operator*(Real v) const { return {x() * v, y() * v}; }
    Vector2 operator/(Real v) const { return {x() / v, y() / v}; }

    Vector2& operator+=(const Vector2& v)
    {
        (*this) = (*this) + v;
        return *this;
    }
    Vector2& operator-=(const Vector2& v)
    {
        (*this) = (*this) - v;
        return *this;
    }
    Vector2& operator*=(const Vector2& v)
    {
        (*this) = (*this) * v;
        return *this;
    }
    Vector2& operator/=(const Vector2& v)
    {
        (*this) = (*this) / v;
        return *this;
    }

    constexpr Real& operator[](Index axis) { return xyz[axis]; }
    constexpr const Real& operator[](Index axis) const { return xyz[axis]; }

    constexpr Real& x() { return xyz[0]; }
    constexpr Real& y() { return xyz[1]; }

    constexpr const Real& x() const { return xyz[0]; }
    constexpr const Real& y() const { return xyz[1]; }

    bool operator<=(const Vector2& rhs) const
    {
        if (x() > rhs.x()) return false;
        return y() > rhs.y() ? false : true;
    }

private:
    Real xyz[2];
};

///
///
///
class Vector3
{
public:
    constexpr static Index DIM = 3;

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

    constexpr Real& operator[](Index axis) { return xyz[axis]; }
    constexpr const Real& operator[](Index axis) const { return xyz[axis]; }

    constexpr Real& x() { return xyz[0]; }
    constexpr Real& y() { return xyz[1]; }
    constexpr Real& z() { return xyz[2]; }

    constexpr const Real& x() const { return xyz[0]; }
    constexpr const Real& y() const { return xyz[1]; }
    constexpr const Real& z() const { return xyz[2]; }

    bool operator<=(const Vector3& rhs) const
    {
        if (x() > rhs.x()) return false;
        if (y() > rhs.y()) return false;
        return z() > rhs.z() ? false : true;
    }

private:
    Real xyz[3];
};

///
///
///

constexpr Vector2 min(const Vector2& a, const Vector2& b) { return {min(a.x(), b.x()), min(a.y(), b.y())}; }

constexpr Vector2 max(const Vector2& a, const Vector2& b) { return {max(a.x(), b.x()), max(a.y(), b.y())}; }

constexpr Vector3 min(const Vector3& a, const Vector3 b)
{
    return {min(a.x(), b.x()), min(a.y(), b.y()), min(a.z(), b.z())};
}

constexpr Vector3 max(const Vector3& a, const Vector3& b)
{
    return {max(a.x(), b.x()), max(a.y(), b.y()), max(a.z(), b.z())};
}

constexpr Real dot(const Vector3& a, const Vector3& b) 
{ 
    return a.x() * b.x() + a.y() * b.y() + a.z() * b.z(); 
}

constexpr Vector3 corss(const Vector3& a, const Vector3& b)
{
    return {a.y() * b.z() - a.z() * b.y(), a.z() * b.x() - a.x() * b.z(), a.x() * b.y() - a.y() * b.x()};
}

inline Vector3 normalize(const Vector3& v)
{
    auto sqr = sqrt(dot(v, v));
    return v / sqr;
}

///
/// Arrays
///
using Vector2Array = std::vector<Vector2>;
using Vector3Array = std::vector<Vector3>;

#endif // LINEAR_VECTOR_HPP
