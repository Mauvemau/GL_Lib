#pragma once
#include "transform.h"

namespace gllib {

    struct DLLExport Material {
    public:
        Vector3 ambient;
        Vector3 diffuse;
        Vector3 specular;
        float shininess;

        Material(const Vector3 ambient, const Vector3 diffuse, const Vector3 specular, const float shininess) {
            this->ambient = ambient;
            this->diffuse = diffuse;
            this->specular = specular;
            this->shininess = shininess;
        }
    };

}
