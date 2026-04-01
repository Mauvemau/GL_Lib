#pragma once
#include "deps.h"
#ifdef _WIN32 // Directory is different in linux
#include "glm.hpp"
#else
#include "glm/glm.hpp"
#endif // _WIN32

namespace gllib
{
    struct DLLExport Vector3 {
        
        float x; // width | pitch
        float y; // height | yaw
        float z; // depth | roll

        explicit operator glm::vec3() const {
            return {x, y, z};
        }

        Vector3 operator*(float scalar)
        {
            return {x * scalar, y * scalar, z * scalar};
        }

        Vector3 operator+(const Vector3& other) const
        {
            return { x + other.x, y + other.y, z + other.z };
        }

        Vector3 operator-(const Vector3& other) const
        {
            return { x - other.x, y - other.y, z - other.z };
        }

        Vector3& operator-=(const Vector3& other)
        {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        }

        Vector3& operator+=(Vector3 vector)
        {
            x += vector.x;
            y += vector.y;
            z += vector.z;
            return *this;
        }

        Vector3 operator/(float scalar)
        {
            return {x / scalar, y / scalar, z / scalar};
        }

        Vector3 operator-( ) const
        {
            return { -x, -y, -z };
        }

        float length() {
            return std::sqrt(x * x + y * y + z * z);
        }

        Vector3 normalized() {
            float len = length();
            if (len == 0.0f) return {0.0f, 0.0f, 0.0f};
            return {x / len, y / len, z / len};
        }


        Vector3(float iX, float iY, float iZ)
        {
            x = iX;
            y = iY;
            z = iZ;
        }

        Vector3()
        {
            x = 0;
            y = 0;
            z = 0;
        }
    };

    struct DLLExport Quaternion
    {
        float w;
        float x;
        float y;
        float z;
        
        Quaternion& operator+=(const Quaternion& quaternion)
        {
            w += quaternion.w;
            x += quaternion.x;
            y += quaternion.y;
            z += quaternion.z;
            return *this;
        }
    };

    struct DLLExport Color
    {
        float r;
        float g;
        float b;
        float a;

        void normalize()
        {
            if (r < 0 || g < 0 || b < 0 || a < 0)
            {
                r = 0.0f;
                g = 0.0f;
                b = 0.0f;
                a = 0.0f;
            }

            if (r > 1.0f || g > 1.0f || b > 1.0f || a > 1.0f)
            {
                r = 1.0f;
                g = 1.0f;
                b = 1.0f;
                a = 1.0f;
            }
        }
    };

    struct DLLExport Transform
    {
        Vector3 position;
        Vector3 scale;
        Quaternion rotationQuat;

        Vector3 forward;
        Vector3 upward;
        Vector3 right;
        Transform operator/(float i) 
        {
            return {
                position / i,
                scale / i,
                {rotationQuat.w / i, rotationQuat.x / i, rotationQuat.y / i, rotationQuat.z / i},
                forward / i,
                upward / i,
                right / i
            };
        }

        Transform operator*(float i)
        {
            return {
                position * i,
                scale * i,
                {rotationQuat.w * i, rotationQuat.x * i, rotationQuat.y * i, rotationQuat.z * i},
                forward * i,
                upward * i,
                right * i
            };
        }    };
    
    struct DLLExport ModelMatrix
    {
        glm::mat4 model;
        glm::mat4 translate;
        glm::mat4 rotationX;
        glm::mat4 rotationY;
        glm::mat4 rotationZ;
        glm::mat4 scale;
    };
    
}
        
