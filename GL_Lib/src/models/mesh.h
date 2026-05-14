#pragma once

#include <vector>

#include "deps.h"
#include "transform.h"

namespace gllib {
    struct DLLExport Vertex {
        Vector3 position;
        Vector3 normal;
        Color color;
        Vector2 uv;
    };

    class DLLExport Mesh {
        private:
        std::vector<Vertex> vertices;
        std::vector<int> indices;

        public:
        Mesh(const std::vector<Vertex>& vertices, const std::vector<int>& indices);
        ~Mesh();

        std::vector<Vertex>& getVertices();
        std::vector<int>& getIndices();
    };
}