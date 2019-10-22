cmake_minimum_required(VERSION 3.8)
project(KdTree CXX)

add_subdirectory(submodules)

add_library(KdTree INTERFACE)
target_include_directories(KdTree
    INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}/include/"
    )

target_sources(KdTree INTERFACE
        # Primitives
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Primitive/Types.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Primitive/Bound.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Primitive/Range.hpp

        # Linear
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Linear/Vector.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Linear/AABBox.hpp

        # Geometry
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Geometry/Mesh.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Geometry/MeshAttrib.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Geometry/MeshOp.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Geometry/PointCloud.hpp

        # Sampling
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Sampling/Vector.hpp

        # Acceleration
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Acceleration/PointKdTree.hpp
        )

target_compile_features(KdTree INTERFACE cxx_std_14)

add_subdirectory(source)
add_subdirectory(tests)
