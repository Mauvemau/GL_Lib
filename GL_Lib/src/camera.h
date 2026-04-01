#pragma once
#include "renderer.h"
#include "myMaths.h"

namespace gllib {

    class DLLExport Camera {
    protected:
        Vector3 position;
        Vector3 direction;
    public:
        Camera();
        virtual ~Camera();

        Vector3 forward();
        static Vector3 forwardWorld();
        Vector3 right();
        static Vector3 rightWorld();
        Vector3 up();
        static Vector3 upWorld();

        void move(Vector3 direction);
        void rotate(Vector3 eulerRotation);

        void update();
    };

}