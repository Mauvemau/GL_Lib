#pragma once

#include "light.h"

namespace gllib {

    class DLLExport PointLight : public Light {
    private:
        Vector3 position;

        float constant;
        float linear;
        float quadratic;
    public:
        PointLight(Vector3 position);
        PointLight(Vector3 position, float constant, float linear, float quadratic);
        PointLight(Vector3 position, float constant, float linear, float quadratic, Color color);
        ~PointLight() override;

        float getConstant() const;
        float getLinear() const;
        float getQuadratic() const;
        Vector3 getPosition() const;

        void setConstant(float constant);
        void setLinear(float linear);
        void setQuadratic(float quadratic);
        void setPosition(Vector3 position);
        void move(Vector3 direction);
    };

}