#ifndef GEOMETRY_MESHOP_HPP
#define GEOMETRY_MESHOP_HPP

#include "Mesh.hpp"

///
/// Mesh Op
///
class MeshOp
{
public:
    enum class Status
    {
        Success,
        Warning,
        Error,
    };

    virtual Status Run(Mesh& mesh) = 0;

protected:
    MeshOp() {}
};

///
/// Mesh Ops
///
class MeshOpGroup : public MeshOp
{
public:
    template <typename T, typename... _Types>
    T* Append(_Types... args)
    {
        ops_.push_back(std::make_unique<T>(std::forward<_Types>(args)...));
        return dynamic_cast<T*>(ops_.back().get());
    }

    Status Run(Mesh& mesh) override
    {
        for (auto& op : ops_)
        {
            auto status = op->Run(mesh);
            if (status == Status::Error) { return status; }
        }

        return Status::Success;
    }

private:
    std::vector<std::unique_ptr<MeshOp>> ops_;
};

#endif // GEOMETRY_MESHOP_HPP
