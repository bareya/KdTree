#ifndef GEOMETRY_POINT_CLOUD_HPP
#define GEOMETRY_POINT_CLOUD_HPP

#include <Primitive/Vector.hpp>

class PointCloud
{
public:
    PointCloud() = default;

    Vector3Array positions;
};

#endif // GEOMETRY_POINT_CLOUD_HPP