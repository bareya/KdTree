#ifndef GEOMETRY_MESH_HPP
#define GEOMETRY_MESH_HPP

#include <Primitive/Vector.hpp>
#include <Primitive/AABBox.hpp>

class Mesh
{
public:
    Mesh(Vector3Array&& vertices, IndexArray&& counts, IndexArray&& indices, IndexArray&& offsets)
        : vertices_(std::move(vertices))
        , counts_(std::move(counts))
        , indices_(std::move(indices))
        , offsets_(std::move(offsets))
    {
        bbox_ = ComputeBBox();
    }

    Mesh(Vector3Array&& vertices, IndexArray&& counts, IndexArray&& indices)
        : vertices_(std::move(vertices))
        , counts_(std::move(counts))
        , indices_(std::move(indices))
    {
        auto num_polygons = NumPolygons();
        offsets_.reserve(num_polygons);
        for (Index poly{}, off{}; poly < num_polygons; off += counts_[poly], ++poly)
        {
            offsets_.push_back(off);
        }
        bbox_ = ComputeBBox();
    }

    Mesh Copy(const Mesh& mesh) const { return Mesh(mesh); }

    const Vector3& GetVertex(Index vertex) const { return vertices_[vertex]; }
    const Vector3& GetVertex(Index polygon, Index vertex) const { return vertices_[offsets_[polygon] + vertex]; }

    Index NumVertices() const { return static_cast<Index>(vertices_.size()); }
    Index NumPolygons() const { return static_cast<Index>(counts_.size()); }
    Index NumPolygonVertices(Index polygon) const { return counts_[polygon]; }

    const AABBox3& GetBounds() const { return bbox_; }

private:
    Mesh(const Mesh&) = default;
    Mesh(Mesh&&) = default;

    Mesh& operator=(const Mesh&) = default;
    Mesh& operator=(Mesh&&) = default;

    AABBox3 ComputeBBox() const
    {
        auto bbox = AABBox3{Vector3{REAL_MAX}, Vector3{REAL_MIN}};
        for(const auto&vertex : vertices_)
        {
            bbox = bbox_.Union(vertex);
        }
        return bbox;
    }

    AABBox3 bbox_;

    Vector3Array vertices_;
    IndexArray counts_;
    IndexArray indices_;
    IndexArray offsets_;
};

#endif // GEOMETRY_MESH_HPP