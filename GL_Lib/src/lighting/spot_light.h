#pragma once

#include "light.h"

namespace gllib {

    class DLLExport SpotLight : public Light {
        private:
        Vector3 position;
        Vector3 direction;

        float cutOff;
        float outerCutOff;

        float constant;
        float linear;
        float quadratic;
        public:
        SpotLight(Vector3 position, Vector3 direction);
        SpotLight(Vector3 position, Vector3 direction, Color color);
        SpotLight(Vector3 position, Vector3 direction, float cutOff, float outerCutOff, float constant, float linear, float quadratic);
        SpotLight(Vector3 position, Vector3 direction, float cutOff, float outerCutOff, float constant, float linear, float quadratic, Color color);
        ~SpotLight() override;

        float getCutOffCos() const;
        float getOuterCutOffCos() const;
        float getConstant() const;
        float getLinear() const;
        float getQuadratic() const;
        Vector3 getPosition() const;
        Vector3 getDirection() const;

        void setCutOff(float degrees);
        void setOuterCutOff(float degrees);
        void setConstant(float constant);
        void setLinear(float linear);
        void setQuadratic(float quadratic);
        void setPosition(Vector3 position);
        void setDirection(Vector3 direction);
        void move(Vector3 direction);
    };

}