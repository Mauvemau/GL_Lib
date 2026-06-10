#pragma once
#include "../transform.h"

namespace gllib {

    struct DLLExport Material {
        Vector3 ambient  {0.35f, 0.35f, 0.35f};
        Vector3 diffuse  {1.0f, 1.0f, 1.0f};
        Vector3 specular {0.5f, 0.5f, 0.5f};

        float shininess = 80.0f;

        float alpha = 1.0f;

        unsigned int texture = 0;

        Material() = default;

        Material(unsigned int texture) : texture(texture) {}

        Material(const Vector3& ambient,const Vector3& diffuse,const Vector3& specular,float shininess,float alpha = 1.0f,unsigned int texture = 0) :
            ambient(ambient),
            diffuse(diffuse),
            specular(specular),
            shininess(shininess),
            alpha(alpha),
            texture(texture) {
        }
    };
}
