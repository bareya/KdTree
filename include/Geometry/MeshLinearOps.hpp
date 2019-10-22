#ifndef GEOMETRY_MESH_LINEAR_OPS_HPP
#define GEOMETRY_MESH_LINEAR_OPS_HPP

#include "MeshOp.hpp"

///
/// Calculate centroid for entire mesh
///
class MeshCentroidOp : public MeshOp
{
public:
    Status Run(Mesh& mesh) override
    {
        Vector3 centroid{};
        for (Index i{}; i < mesh.NumPoints(); ++i)
        {
            centroid += mesh.GetPosition(i);
        }
        centroid = centroid / static_cast<Real>(mesh.NumPoints());

        centroid_attrib = mesh.CreateMeshAttrib<MeshVector3Attrib>(MeshAttribNames::Centroid);
        centroid_attrib->Get(0) = centroid;

        return Status::Success;
    }

    /// TODO: saving members is making evalution not thread safe
    MeshVector3Attrib* centroid_attrib{};
};

///
/// Calculate normal of entire mesh
///
class MeshNormalOp : public MeshOp
{
public:
    MeshNormalOp(MeshAttribRank rank = MeshAttribRank::Polygon)
        : MeshOp()
        , rank_(rank)
    {
    }

    Status Run(Mesh& mesh) override
    {
        normal_attrib = mesh.CreatePolygonAttrib<MeshVector3Attrib>(MeshAttribNames::Normal);

        for (Index i{}; i < mesh.NumPolygons(); ++i)
        {
            auto num_vertices = mesh.NumPolygonVertices(i);
            if (num_vertices < 3) continue;

            const auto& p0 = mesh.GetPosition(i, 0);
            const auto& p1 = mesh.GetPosition(i, 1);
            const auto& p2 = mesh.GetPosition(i, 2);

            auto v01 = p0 - p1;
            auto v21 = p2 - p1;

            normal_attrib->Get(i) = normalize(cross(v01, v21));
        }

        return Status::Success;
    }

    // cached handle
    MeshVector3Attrib* normal_attrib{};

private:
    MeshAttribRank rank_;
};

///
/// Computes Area Attribute for each polygon
///
class MeshAreaOp : public MeshOp
{
public:
    Status Run(Mesh& mesh) override
    {
        // create storage
        MeshRealAttrib* poly_area_attrib = mesh.CreatePolygonAttrib<MeshRealAttrib>("area");
        MeshRealAttrib* mesh_area_attrib = mesh.CreateMeshAttrib<MeshRealAttrib>("area");

        // get position
        const Vector3& p0 = mesh.GetPosition(0);
        const Vector3& p1 = mesh.GetPosition(1);
        const Vector3& p2 = mesh.GetPosition(2);

        // xy
        Vector3 p0p1xy = p1 - p0;
        Vector3 p1p2xy = p2 - p1;

        Real xy = 0.5_r * (p0p1xy.x() * p1p2xy.y() - p0p1xy.y() * p1p2xy.x());
        Real yz = 0.5_r * (p0p1xy.y() * p1p2xy.z() - p0p1xy.z() * p1p2xy.y());
        Real zx = 0.5_r * (p0p1xy.z() * p1p2xy.x() - p0p1xy.x() * p1p2xy.z());

        // 0.30831
        return Status::Success;
    }

private:
    Real ComputeAxisArea() const {}
};

#endif GEOMETRY_MESH_LINEAR_OPS_HPP