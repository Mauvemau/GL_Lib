#pragma once
#include "myMaths.h"

namespace gllib {

    class DLLExport Camera {
    protected:
        Vector3 position;
        Vector3 direction;
    public:
        Camera();
        Camera(Vector3 position, Vector3 direction);
        virtual ~Camera();

        Vector3 forward();
        static Vector3 forwardWorld();
        Vector3 right();
        static Vector3 rightWorld();
        Vector3 up();
        static Vector3 upWorld();

        void move(Vector3 direction);
        void rotate(float x, float y);
        void lookAt(Vector3 target);

        void update();
    };

}