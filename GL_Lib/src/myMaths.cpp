#include "myMaths.h"

#include <cmath>


namespace gllib{
     Vector3 Maths::cross(Vector3 a, Vector3 b) {
        return {
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        };
    }

    float Maths::dot(Vector3 a, Vector3 b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    float Maths::clamp(float value, float min, float max) {
         if (value < min) return min;
         if (value > max) return max;
         return value;
     }

    float Maths::deg2Rad()
    {
        constexpr float pi = 3.14159265359f;

        return (pi * 2.0f) / 360.0f;
    }

    Quaternion Maths::Euler(Vector3 euler)
    {
        euler.x *= deg2Rad();
        euler.y *= deg2Rad();
        euler.z *= deg2Rad();

        float cy = cos(euler.z * 0.5f);
        float sy = sin(euler.z * 0.5f);
        float cp = cos(euler.x * 0.5f);
        float sp = sin(euler.x * 0.5f);
        float cr = cos(euler.y * 0.5f);
        float sr = sin(euler.y * 0.5f);

        Quaternion q;
        q.w = cr * cp * cy + sr * sp * sy;
        q.x = cr * sp * cy + sr * cp * sy;
        q.y = sr * cp * cy - cr * sp * sy;
        q.z = cr * cp * sy - sr * sp * cy;
        return q;
    }

    Vector3 Maths::Quat2Vec3(Quaternion Quat, Vector3 Vec)
    {
        float x2 = Quat.x * 2.0f;
        float y2 = Quat.y * 2.0f;
        float z2 = Quat.z * 2.0f;
        float xx2 = Quat.x * x2;
        float yy2 = Quat.y * y2;
        float zz2 = Quat.z * z2;
        float xy2 = Quat.x * y2;
        float xz2 = Quat.x * z2;
        float yz2 = Quat.y * z2;
        float wx2 = Quat.w * x2;
        float wy2 = Quat.w * y2;
        float wz2 = Quat.w * z2;

        Vector3 result;
        result.x = (1.0f - (yy2 + zz2)) * Vec.x + (xy2 - wz2) * Vec.y + (xz2 + wy2) * Vec.z;
        result.y = (xy2 + wz2) * Vec.x + (1.0f - (xx2 + zz2)) * Vec.y + (yz2 - wx2) * Vec.z;
        result.z = (xz2 - wy2) * Vec.x + (yz2 + wx2) * Vec.y + (1.0f - (xx2 + yy2)) * Vec.z;
        return result;
    }

    Vector3 Maths::QuatToEuler(const Quaternion& q) {
         Vector3 euler;

         // Yaw (Y axis)
         float siny_cosp = 2.0f * (q.w * q.y + q.z * q.x);
         float cosy_cosp = 1.0f - 2.0f * (q.y * q.y + q.x * q.x);
         euler.y = std::atan2(siny_cosp, cosy_cosp);

         // Pitch (X axis)
         float sinp = 2.0f * (q.w * q.x - q.z * q.y);
         if (std::fabs(sinp) >= 1)
             euler.x = copysign(M_PI / 2, sinp);
         else
             euler.x = std::asin(sinp);

         // Roll (Z axis)
         float sinr_cosp = 2.0f * (q.w * q.z + q.x * q.y);
         float cosr_cosp = 1.0f - 2.0f * (q.z * q.z + q.x * q.x);
         euler.z = std::atan2(sinr_cosp, cosr_cosp);

         return euler;
     }
}
