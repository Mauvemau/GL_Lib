#pragma once

#include "transform.h"

namespace gllib
{
    class DLLExport Maths
    {
    public:
        static Vector3 cross(Vector3 a, Vector3 b);
        static float dot(Vector3 a, Vector3 b);
        static float clamp(float value, float min, float max);
        static Quaternion Euler(Vector3 euler);
        static float deg2Rad();
        static Vector3 Quat2Vec3(Quaternion Quat, Vector3 Vec);
    };
}
