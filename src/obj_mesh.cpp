/**
 * @author	Andrew Robert Owens, Philmo Gu
 * @date 	2019-10-15.
 * @details Organization: Biological Modeling and Visualization research group
 * University of Calgary, Calgary, AB, Canada
 *
 * Contact:	arowens [at] ucalgary.ca
 * @copyright Copyright (c) 2019 ___ALGORITHMIC_BOTANY___. All rights reserved.
 *
 * @brief
 */

#include "obj_mesh.hpp"

#include <algorithm>
#include <cassert>

using namespace math;

namespace geometry {

std::vector<math::Vec3f>
calculateTriangleNormals(std::vector<IndicesTriangle> const &indexTriangles,
                         std::vector<math::Vec3f> const &vertices) {
    std::vector<math::Vec3f> tmp;

    for (auto& triangle : indexTriangles) {

        math::Vec3f  a, b, c;

        auto& ai = triangle.a();
        auto& bi = triangle.b();
        auto& ci = triangle.c();

        a = vertices[ai.vertexID()];
        b = vertices[bi.vertexID()];
        c = vertices[ci.vertexID()];

        math::Vec3f norm = cross(b - a, c - a);

        tmp.push_back(norm);

    }
    //std::cout << tmp.size() << "\n";
    return tmp;
}

std::vector<math::Vec3f>
calculateVertexNormals(std::vector<IndicesTriangle> const &indexTriangles,
                       std::vector<math::Vec3f> const &vertices) {
    return calculateVertexNormals(
            indexTriangles,
            vertices,
            calculateTriangleNormals(
                    indexTriangles,
                    vertices)
    );
}

std::vector<math::Vec3f>
calculateVertexNormals(std::vector<IndicesTriangle> const &indexTriangles,
                       std::vector<math::Vec3f> const &vertices,
                       std::vector<math::Vec3f> const &triangleNormals) {
    std::vector<std::vector<math::Vec3f>> vertStorage = std::vector<std::vector<math::Vec3f>>(vertices.size());
    std::vector<math::Vec3f> tmp;
    for (long i = 0; i < indexTriangles.size(); i++)
    {
        IndicesTriangle tri = indexTriangles.at(i);
        math::Vec3f tNorm = triangleNormals.at(i);
        //std::cout << tNorm << "\n";
        vertStorage[tri.a().vertexID()].push_back(tNorm);
        vertStorage[tri.b().vertexID()].push_back(tNorm);
        vertStorage[tri.c().vertexID()].push_back(tNorm);

    }

    for (auto tNorms : vertStorage)
    {
        math::Vec3f v;
        int count = 0;
        for (auto vi : tNorms)
        {
            v += (vi);
            count++;
        }
        tmp.push_back(v.normalize());
    }

    return tmp;
}

} // namespace geometry
