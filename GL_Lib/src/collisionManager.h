#pragma once
#include <vector>

#include "rectangle.h"

namespace gllib
{
    class DLLExport collisionManager
    {
    public:
        collisionManager(const std::vector<Entity*>& obstacles);
        ~collisionManager();
        bool checkCollision(Entity* player);
        bool checkCollision(Transform transform);
        void addObstacle(Entity* obstacle);
        void removeObstacle(Entity* obstacle_);

    private:
        std::vector<Entity*> obstacles;
    };
}
