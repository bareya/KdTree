#ifndef POINT2D_H
#define POINT2D_H

#include <vector>
#include <random>
#include <ostream>

struct Point2D
{
    explicit Point2D(float xx = 0.0f, float yy = 0.0f)
        : x(xx)
        , y(yy)
    {
    }

    float x, y;
};

inline std::ostream& operator<<(std::ostream& os, const Point2D& point)
{
    os << point.x << " " << point.y;
    return os;
}

template <> struct KdMin<Point2D>
{
    static Point2D value(const Point2D& l, const Point2D& r) { return Point2D{std::min(l.x, r.x), std::min(l.y, r.y)}; }
};

template <> struct KdMax<Point2D>
{
    static Point2D value(const Point2D& l, const Point2D& r) { return Point2D{std::max(l.x, r.x), std::max(l.y, r.y)}; }
};

template <> struct KdComparator<Point2D>
{
    KdComparator(const std::vector<Point2D>& p, KdIndex d)
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
        default:
            return points[l].x < points[r].x;
        }
    }

private:
    const std::vector<Point2D>& points;
    const KdIndex dimension;
};

//
// Random point generator
//

inline void generate_random_points(std::vector<Point2D>& points, size_t size, const Point2D& mn = Point2D{0, 0},
                                   const Point2D& mx = Point2D{1, 1})
{
    points.reserve(size);

    std::mt19937 mt{0};
    std::uniform_real_distribution<float> dist_x{mn.x, mx.x};
    std::uniform_real_distribution<float> dist_y{mn.y, mx.y};

    for (size_t i{}; i < size; ++i)
    {
        points.emplace_back(dist_x(mt), dist_y(mt));
    }
}

#endif // POINT2D_H