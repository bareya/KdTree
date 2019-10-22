#ifndef GEOMETRY_MESHATTRIB_HPP
#define GEOMETRY_MESHATTRIB_HPP

#include <Primitive/Types.hpp>
#include <Linear/Vector.hpp>

#include <memory>

class Mesh;

enum class MeshAttribRank
{
    Point,
    Vertex,
    Polygon,
    Mesh
};

enum class MeshAttribStorage
{
    Real,
    Index,
    Vector2,
    Vector3
};

enum class MeshAttribAccess
{
    Private,
    Public
};

///
///
///
class MeshAttrib
{
public:
    virtual ~MeshAttrib() = default;

    MeshAttribRank GetRank() const { return rank_; }
    MeshAttribStorage GetStorage() const { return storage_; }
    Index GetSize() const { return size_; }

protected:
    MeshAttrib(Mesh& mesh, MeshAttribRank rank, MeshAttribStorage storage, Index size)
        : mesh_(mesh)
        , rank_(rank)
        , storage_(storage)
        , size_(size)
    {
    }

private:
    Mesh& mesh_;
    MeshAttribRank rank_;
    MeshAttribStorage storage_;
    Index size_;
};

using MeshAttribPtr = std::unique_ptr<MeshAttrib>;
using MeshAttribMap = std::map<std::string, MeshAttribPtr>;

///
///
///

template <typename T>
struct TypeToStorageConverter
{
};

template <>
struct TypeToStorageConverter<Real>
{
    static MeshAttribStorage value() { return MeshAttribStorage::Real; }
};

template <>
struct TypeToStorageConverter<Index>
{
    static MeshAttribStorage value() { return MeshAttribStorage::Index; }
};

template <>
struct TypeToStorageConverter<Vector2>
{
    static MeshAttribStorage value() { return MeshAttribStorage::Vector2; }
};

template <>
struct TypeToStorageConverter<Vector3>
{
    static MeshAttribStorage value() { return MeshAttribStorage::Vector3; }
};

///
///
///
template <typename T>
class MeshNumericAttrib : public MeshAttrib
{
public:
    using value_type = T;

    template <typename... _Types>
    static std::unique_ptr<MeshNumericAttrib<T>> Create(_Types&&... _Args)
    {
        return std::make_unique<MeshNumericAttrib<T>>(std::forward<_Types>(_Args)...);
    }

    MeshNumericAttrib(Mesh& mesh, MeshAttribRank rank, Index size)
        : MeshAttrib(mesh, rank, TypeToStorageConverter<T>::value(), size)
    {
        data.resize(size);
    }

    MeshNumericAttrib(Mesh& mesh, MeshAttribRank rank, std::vector<T>&& new_data)
        : MeshAttrib(mesh, rank, TypeToStorageConverter<T>::value(), new_data.size())
        , data(std::move(new_data))
    {
    }

    const T& Get(Index i) const { return data[i]; }
    T& Get(Index i) { return data[i]; }

private:
    std::vector<T> data;
};

///
/// Numeric Type definitions
///
using MeshRealAttrib = MeshNumericAttrib<Real>;
using MeshIndexAttrib = MeshNumericAttrib<Index>;
using MeshVector2Attrib = MeshNumericAttrib<Vector2>;
using MeshVector3Attrib = MeshNumericAttrib<Vector3>;

///
/// Mesh Attrib Names
///
namespace MeshAttribNames
{
const char* const Position{"position"};
const char* const PolyVertexCount{"poly_vertex_count"};
const char* const VertexPointIndex{"vertex_point_index"};
const char* const PolyVertexOffset{"poly_vertex_offset"};
const char* const Normal{"normal"};
const char* const Area{"area"};
const char* const Density{"density"};
const char* const Centroid{"centroid"};
} // namespace MeshAttribNames

#endif // GEOMETRY_MESHATTRIB_HPP
