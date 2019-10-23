#ifndef GEOMETRY_MESH_HPP
#define GEOMETRY_MESH_HPP

#include <Linear/Vector.hpp>
#include <Linear/AABBox.hpp>

#include "MeshAttrib.hpp"

#include <map>
#include <memory>

///
/// Immutable Mesh
///
class Mesh
{
public:
    Mesh(Vector3Array&& position, IndexArray&& poly_vertex_count, IndexArray&& vertex_point_index)
        : rank_attrib_map_{*this}
    {
        // position
        position_ = rank_attrib_map_.CreateAttrib<MeshVector3Attrib>(
            MeshAttribNames::Position,
            MeshAttribRank::Point,
            std::move(position));

        poly_vertex_count_ = rank_attrib_map_.CreateAttrib<MeshIndexAttrib>(
            MeshAttribNames::PolyVertexCount,
            MeshAttribRank::Polygon, 
            std::move(poly_vertex_count)
        );

        vertex_point_index_ = rank_attrib_map_.CreateAttrib<MeshIndexAttrib>(
            MeshAttribNames::VertexPointIndex,
            MeshAttribRank::Vertex, 
            std::move(vertex_point_index)
        );

        // compute offset
        IndexArray poly_offsets;
        poly_offsets.reserve(NumPolygons());
        for (Index polygon{}, offset{}; polygon < NumPolygons(); offset += poly_vertex_count_->Get(polygon), ++polygon)
        {
            poly_offsets.emplace_back(offset);
        }
        poly_vertex_offset_ = rank_attrib_map_.CreateAttrib<MeshIndexAttrib>(
            MeshAttribNames::PolyVertexOffset,
            MeshAttribRank::Polygon, 
            std::move(poly_offsets)
        );
    }

    // General information
    Index NumElements(MeshAttribRank rank) const { return 0; }

    Index NumPoints() const { return position_->GetSize(); }
    Index NumVertices() const { return vertex_point_index_->GetSize(); }
    Index NumPolygons() const { return poly_vertex_count_->GetSize(); }
    Index NumPolygonVertices(Index polygon) const { return poly_vertex_count_->Get(polygon); }

    // position accessor
    const Vector3& GetPosition(Index point) const { return position_->Get(point); }
    const Vector3& GetPosition(Index polygon, Index vertex) const
    {
        assert(polygon != NumPolygons());
        const Index point_offset = poly_vertex_offset_->Get(polygon) + vertex;
        return position_->Get(point_offset);
    }

    // Custom Mesh Attributes
    template <typename T>
    T* CreatePointAttrib(const char* name)
    {
        return CreateAttrib<T>(name, Mesh::MeshAttribRank::Point, NumPoints());
    }

    template <typename T>
    T* CreateVertexAttrib(const char* name)
    {
        return CreateAttrib<T>(name, Mesh::MeshAttribRank::Vertex, NumVertices());
    }

    template <typename T>
    T* CreatePolygonAttrib(const char* name)
    {
        return CreateAttrib<T>(name, MeshAttribRank::Polygon, NumPolygons());
    }

    template <typename T>
    T* CreateMeshAttrib(const char* name)
    {
        return CreateAttrib<T>(name, MeshAttribRank::Mesh, 1);
    }

    MeshAttrib* FindAttrib(const char* name, MeshAttribRank rank);

private:
    Mesh(const Mesh&) = default;
    Mesh(Mesh&&) = default;

    Mesh& operator=(const Mesh&) = default;
    Mesh& operator=(Mesh&&) = default;

    MeshAttribMap& GetAttribMap(MeshAttribRank rank);

    template <typename T>
    T* CreateAttrib(const char* name, MeshAttribRank rank, Index size)
    {
        return rank_attrib_map_.CreateAttrib<T>(name, rank, size);
    }

    // cached handles
    MeshVector3Attrib* position_{};
    MeshIndexAttrib* poly_vertex_count_{};
    MeshIndexAttrib* poly_vertex_offset_{};
    MeshIndexAttrib* vertex_point_index_{};

    MeshRankAttribMap rank_attrib_map_;
};

inline MeshAttribMap& Mesh::GetAttribMap(MeshAttribRank rank) { return rank_attrib_map_.GetAttribMap(rank); }

inline MeshAttrib* Mesh::FindAttrib(const char* name, MeshAttribRank rank)
{
    return rank_attrib_map_.FindAttrib(name, rank);
}

#endif // GEOMETRY_MESH_HPP