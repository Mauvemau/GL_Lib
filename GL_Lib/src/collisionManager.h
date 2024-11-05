#pragma once
#include <vector>

#include "Rectangle.h"

namespace gllib
{
    class collisionManager
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
