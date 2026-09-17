#pragma once

#ifdef _WIN32
#include <glm.hpp>
#else
#include <glm/glm.hpp>
#endif

#include "models/model_node.h"

namespace gllib {

    struct DLLExport Frustum {
        glm::vec4 planes[6];
        glm::vec3 cameraPosition{0.0f};

        enum Planes {
            LEFT = 0,
            RIGHT,
            BOTTOM,
            TOP,
            NEAR_PLANE,
            FAR_PLANE
        };

        void update(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
            glm::mat4 invView = glm::inverse(viewMatrix);
            cameraPosition = glm::vec3(invView[3]);

            glm::mat4 viewProj = projectionMatrix * viewMatrix;
            planes[LEFT].x = viewProj[0][3] + viewProj[0][0];
            planes[LEFT].y = viewProj[1][3] + viewProj[1][0];
            planes[LEFT].z = viewProj[2][3] + viewProj[2][0];
            planes[LEFT].w = viewProj[3][3] + viewProj[3][0];

            planes[RIGHT].x = viewProj[0][3] - viewProj[0][0];
            planes[RIGHT].y = viewProj[1][3] - viewProj[1][0];
            planes[RIGHT].z = viewProj[2][3] - viewProj[2][0];
            planes[RIGHT].w = viewProj[3][3] - viewProj[3][0];

            planes[BOTTOM].x = viewProj[0][3] + viewProj[0][1];
            planes[BOTTOM].y = viewProj[1][3] + viewProj[1][1];
            planes[BOTTOM].z = viewProj[2][3] + viewProj[2][1];
            planes[BOTTOM].w = viewProj[3][3] + viewProj[3][1];

            planes[TOP].x = viewProj[0][3] - viewProj[0][1];
            planes[TOP].y = viewProj[1][3] - viewProj[1][1];
            planes[TOP].z = viewProj[2][3] - viewProj[2][1];
            planes[TOP].w = viewProj[3][3] - viewProj[3][1];

            planes[NEAR_PLANE].x = viewProj[0][3] + viewProj[0][2];
            planes[NEAR_PLANE].y = viewProj[1][3] + viewProj[1][2];
            planes[NEAR_PLANE].z = viewProj[2][3] + viewProj[2][2];
            planes[NEAR_PLANE].w = viewProj[3][3] + viewProj[3][2];

            planes[FAR_PLANE].x = viewProj[0][3] - viewProj[0][2];
            planes[FAR_PLANE].y = viewProj[1][3] - viewProj[1][2];
            planes[FAR_PLANE].z = viewProj[2][3] - viewProj[2][2];
            planes[FAR_PLANE].w = viewProj[3][3] - viewProj[3][2];

            for (int i = 0; i < 6; i++) {
                float length = glm::length(glm::vec3(planes[i]));
                planes[i] /= length;
            }
        }

        bool isBoxInFrustum(const BoundingBox& box) const {
            for (int i = 0; i < 6; ++i) {
                const glm::vec4& plane = planes[i];

                glm::vec3 positiveVertex;
                positiveVertex.x = (plane.x >= 0.0f) ? box.max.x : box.min.x;
                positiveVertex.y = (plane.y >= 0.0f) ? box.max.y : box.min.y;
                positiveVertex.z = (plane.z >= 0.0f) ? box.max.z : box.min.z;

                float distance = plane.x * positiveVertex.x +
                                 plane.y * positiveVertex.y +
                                 plane.z * positiveVertex.z +
                                 plane.w;

                if (distance < 0.0f) {
                    return false;
                }
            }
            return true;
        }
    };

}