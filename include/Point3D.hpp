#ifndef POINT3D_H
#define POINT3D_H

#include <vector>
#include <ostream>

struct Point3D
{
    explicit Point3D(float xx = 0.0f, float yy = 0.0f, float zz = 0.0f)
        : x(xx)
        , y(yy)
        , z(zz)
    {
    }

    float x, y, z;
};

inline std::ostream& operator<<(std::ostream& os, const Point3D& point)
{
    os << point.x << " " << point.y << " " << point.z;
    return os;
}

template <> struct KdMin<Point3D>
{
    static Point3D value(const Point3D& l, const Point3D& r)
    {
        return Point3D{std::min(l.x, r.x), std::min(l.y, r.y), std::min(l.z, r.z)};
    }
};

template <> struct KdMax<Point3D>
{
    static Point3D value(const Point3D& l, const Point3D& r)
    {
        return Point3D{std::max(l.x, r.x), std::max(l.y, r.y), std::max(l.z, r.z)};
    }
};

template <> struct KdComparator<Point3D>
{
public:
    KdComparator(const std::vector<Point3D>& p, KdIndex d)
        : points(p)
        , dimension(d)
    {
    }
    bool operator()(KdIndex l, KdIndex r) const
    {
        switch (dimension)
        {
        case 0:
            return points[l].x < points[r].x;
        case 1:
            return points[l].y < points[r].y;
        case 2:
            return points[l].z < points[r].z;
        default:
            return points[l].x < points[r].x;
        }
    }

private:
    const std::vector<Point3D>& points;
    const KdIndex dimension;
};

//
// Random point generator
//

inline void generate_random_points(std::vector<Point3D>& points, size_t size, const Point3D& mn = Point3D{0, 0, 0},
                                   const Point3D& mx = Point3D{1, 1, 1})
{
    points.reserve(size);

    std::mt19937 mt{0};
    std::uniform_real_distribution<float> dist_x{mn.x, mx.x};
    std::uniform_real_distribution<float> dist_y{mn.y, mx.y};
    std::uniform_real_distribution<float> dist_z{mn.z, mx.z};

    for (size_t i{}; i < size; ++i)
    {
        points.emplace_back(dist_x(mt), dist_y(mt), dist_z(mt));
    }
}

#endif // POINT3D_H