#ifndef GEOMETRY_MESHATTRIB_HPP
#define GEOMETRY_MESHATTRIB_HPP

#include <Primitive/Types.hpp>
#include <Linear/Vector.hpp>

class Mesh;

///
///
///
class MeshAttrib
{
public:
    enum class Storage
    {
        Real,
        Index,
        Vector2,
        Vector3
    };

    enum class Rank
    {
        Point,
        // Vertex,
        Polygon,
        Mesh
    };

    virtual ~MeshAttrib() = default;

    Storage GetStorage() const { return storage_; }
    Rank GetRank() const { return rank_; }

protected:
    MeshAttrib(Mesh& mesh, Rank rank, Storage storage, Index size)
        : mesh_(mesh)
        , rank_(rank)
        , storage_(storage)
        , size_(size)
    {
    }

private:
    Mesh& mesh_;
    Rank rank_;
    Storage storage_;
    Index size_;
};

template <typename T>
struct TypeToStorageConverter
{
};

template <>
struct TypeToStorageConverter<Real>
{
    static MeshAttrib::Storage value() { return MeshAttrib::Storage::Real; }
};

template <>
struct TypeToStorageConverter<Index>
{
    static MeshAttrib::Storage value() { return MeshAttrib::Storage::Index; }
};

template <>
struct TypeToStorageConverter<Vector2>
{
    static MeshAttrib::Storage value() { return MeshAttrib::Storage::Vector2; }
};

template <>
struct TypeToStorageConverter<Vector3>
{
    static MeshAttrib::Storage value() { return MeshAttrib::Storage::Vector3; }
};

///
///
///
template <typename T>
class MeshNumericAttrib : public MeshAttrib
{
public:
    using value_type = T;

    MeshNumericAttrib(Mesh& mesh, Rank rank, Storage storage, Index size)
        : MeshAttrib(mesh, rank, storage, size)
    {
        data.resize(size);
    }

    std::vector<T> data;
};

///
///
///
using MeshRealAttrib = MeshNumericAttrib<Real>;
using MeshIndexAttrib = MeshNumericAttrib<Index>;
using MeshVector2Attrib = MeshNumericAttrib<Vector2>;
using MeshVector3Attrib = MeshNumericAttrib<Vector3>;

#endif // GEOMETRY_MESHATTRIB_HPP
