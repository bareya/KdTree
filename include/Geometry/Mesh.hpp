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
    {
        // position
        auto position_attrib = MeshVector3Attrib::Create(*this, MeshAttribRank::Point, std::move(position));
        position_ = position_attrib.get();
        auto position_it = point_attribs_.emplace(MeshAttribNames::Position, std::move(position_attrib));

        // poly vertex count
        auto poly_vertex_count_attrib =
            MeshIndexAttrib::Create(*this, MeshAttribRank::Polygon, std::move(poly_vertex_count));
        poly_vertex_count_ = poly_vertex_count_attrib.get();
        polygon_attribs_.emplace(MeshAttribNames::PolyVertexCount, std::move(poly_vertex_count_attrib));

        // vertex point index
        auto vertex_point_index_attrib =
            MeshIndexAttrib::Create(*this, MeshAttribRank::Vertex, std::move(vertex_point_index));
        vertex_point_index_ = vertex_point_index_attrib.get();
        vertex_attribs_.emplace(MeshAttribNames::VertexPointIndex, std::move(vertex_point_index_attrib));

        // compute offset
        IndexArray poly_offsets;
        poly_offsets.reserve(NumPolygons());
        for (Index polygon{}, offset{}; polygon < NumPolygons(); offset += poly_vertex_count_->Get(polygon), ++polygon)
        {
            poly_offsets.emplace_back(offset);
        }
        auto poly_vertex_offset_attrib =
            MeshIndexAttrib::Create(*this, MeshAttribRank::Polygon, std::move(poly_offsets));
        poly_vertex_offset_ = poly_vertex_offset_attrib.get();
        polygon_attribs_.emplace(MeshAttribNames::PolyVertexOffset, std::move(poly_vertex_offset_attrib));
    }

    // General information
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
        // find existing, might return nullptr
        auto found_attrib = FindAttrib(name, rank);
        if (found_attrib) { return dynamic_cast<T*>(found_attrib); }

        // create new
        auto new_attrib = std::make_unique<T>(*this, rank, size);
        auto new_attrib_ptr = new_attrib.get();

        switch (rank)
        {
        case MeshAttribRank::Point:
        {
            point_attribs_.emplace(name, std::move(new_attrib));
            return new_attrib_ptr;
        }
        case MeshAttribRank::Vertex:
        {
            vertex_attribs_.emplace(name, std::move(new_attrib));
            return new_attrib_ptr;
        }
        case MeshAttribRank::Polygon:
        {
            polygon_attribs_.emplace(name, std::move(new_attrib));
            return new_attrib_ptr;
        }
        case MeshAttribRank::Mesh:
        {
            mesh_attribs_.emplace(name, std::move(new_attrib));
            return new_attrib_ptr;
        }
        default:
        {
            return nullptr;
        }
        }
    }

    // cached handles
    MeshVector3Attrib* position_{};
    MeshIndexAttrib* poly_vertex_count_{};
    MeshIndexAttrib* poly_vertex_offset_{};
    MeshIndexAttrib* vertex_point_index_{};

    // attributes
    MeshAttribMap point_attribs_;
    MeshAttribMap vertex_attribs_;
    MeshAttribMap polygon_attribs_;
    MeshAttribMap mesh_attribs_;
};

inline MeshAttribMap& Mesh::GetAttribMap(MeshAttribRank rank)
{
    switch (rank)
    {
    case MeshAttribRank::Point:
        return point_attribs_;
    case MeshAttribRank::Vertex:
        return vertex_attribs_;
    case MeshAttribRank::Polygon:
        return polygon_attribs_;
    case MeshAttribRank::Mesh:
        return mesh_attribs_;
    default:
        throw std::runtime_error("Unrechable block reached!");
    }
}

inline MeshAttrib* Mesh::FindAttrib(const char* name, MeshAttribRank rank)
{
    auto& attrib_map = GetAttribMap(rank);
    auto attrib_it = attrib_map.find(name);
    if (attrib_it == attrib_map.end()) { return nullptr; }
    return attrib_it->second.get();
}

#endif // GEOMETRY_MESH_HPP