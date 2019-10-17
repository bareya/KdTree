#ifndef MATH_AABBOX_H
#define MATH_AABBOX_H

#include "Vector.hpp"
#include "Bound.hpp"

///
/// 2D and 3D Axis Aligned Bounding Box
///
using AABBox2 = BoundT<Vector2>;
using AABBox3 = BoundT<Vector3>;

#endif // MATH_AABBOX_H
