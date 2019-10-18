#ifndef GEOMETRY_MESH_HPP
#define GEOMETRY_MESH_HPP

#include <Linear/Vector.hpp>
#include <Linear/AABBox.hpp>

#include "MeshAttrib.hpp"

#include <map>
#include <memory>

///
///
///
class Mesh
{
public:
    using Rank = MeshAttrib::Rank;
    using Storage = MeshAttrib::Storage;

    Mesh(Vector3Array&& positions, IndexArray&& counts, IndexArray&& indices, IndexArray&& offsets);
    Mesh(Vector3Array&& positions, IndexArray&& counts, IndexArray&& indices);

    //Mesh Copy(const Mesh& mesh) const { return Mesh(mesh); }

    const Vector3& GetVertex(Index vertex) const { return positions_[vertex]; }
    const Vector3& GetVertex(Index polygon, Index vertex) const { return positions_[offsets_[polygon] + vertex]; }

    Index NumPoints() const { return static_cast<Index>(positions_.size()); }
    Index NumPolygons() const { return static_cast<Index>(counts_.size()); }
    Index NumPolygonVertices(Index polygon) const { return counts_[polygon]; }

    const AABBox3& GetBounds() const { return bbox_; }

    template<typename T>
    MeshAttrib* CreateAttribute(const char* name, Rank rank, Storage storage)
    {
        switch(rank)
        {
            case Rank::Point:
            {
                auto new_attrib = std::make_unique<T>(*this, rank, storage, NumPoints());
                auto it = point_attribs_.emplace(name, std::move(new_attrib));
                return it.first->second.get();
            }
            case Rank::Polygon:
            {
                auto new_attrib = std::make_unique<T>(*this, rank, storage, NumPolygons());
                auto it = polygon_attribs_.emplace(name, std::move(new_attrib));
                return it.first->second.get();
            }
            case Rank::Mesh:
            {
                auto new_attrib = std::make_unique<T>(*this, rank, storage, 1);
                auto it = mesh_attribs_.emplace(name, std::move(new_attrib));
                return it.first->second.get();
            }
            default:
            {
                return nullptr;
            }
        }
    }

private:
    Mesh(const Mesh&) = default;
    Mesh(Mesh&&) = default;

    Mesh& operator=(const Mesh&) = default;
    Mesh& operator=(Mesh&&) = default;

    AABBox3 ComputeBBox() const
    {
        auto bbox = AABBox3{Vector3{REAL_MAX}, Vector3{REAL_MIN}};
        for(const auto&vertex : positions_)
        {
            bbox = bbox_.Union(vertex);
        }
        return bbox;
    }

    AABBox3 bbox_;

    Vector3Array positions_;
    IndexArray counts_;
    IndexArray vertices_;
    IndexArray offsets_;

    std::map<std::string, std::unique_ptr<MeshAttrib>> point_attribs_;
    std::map<std::string, std::unique_ptr<MeshAttrib>> polygon_attribs_;
    std::map<std::string, std::unique_ptr<MeshAttrib>> mesh_attribs_;
};

Mesh::Mesh(Vector3Array&& positions, IndexArray&& counts, IndexArray&& indices, IndexArray&& offsets)
    : positions_(std::move(positions))
    , counts_(std::move(counts))
    , vertices_(std::move(indices))
    , offsets_(std::move(offsets))
{
    bbox_ = ComputeBBox();
}

Mesh::Mesh(Vector3Array&& positions, IndexArray&& counts, IndexArray&& indices)
    : positions_(std::move(positions))
    , counts_(std::move(counts))
    , vertices_(std::move(indices))
{
    auto num_polygons = NumPolygons();
    offsets_.reserve(num_polygons);
    for (Index poly{}, off{}; poly < num_polygons; off += counts_[poly], ++poly)
    {
        offsets_.push_back(off);
    }
    bbox_ = ComputeBBox();
}

#endif // GEOMETRY_MESH_HPP