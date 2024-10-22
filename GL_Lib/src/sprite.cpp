#include "sprite.h"
#include "loader.h"

using namespace gllib;
using namespace std;

Sprite::Sprite(Vector3 translation, Vector3 rotation, Vector3 scale, Color color) :
    Shape(translation, rotation, scale) {
    this->color = color;
    updateRenderData(this->color);
    cout << "Created rectangle.\n";
}

Sprite::Sprite(Transform transform, Color color) :
    Shape(transform) {
    this->color = color;
    updateRenderData(this->color);
    cout << "Created rectangle.\n";
}

Sprite::~Sprite() {
    cout << "Destroyed rectangle.\n";
}

void Sprite::updateRenderData(Color color) {
    // Values for the vertices
    float rectangleVertexData[] = {
         1.0f, 1.0f, 0.0f, /* xyz */ color.r, color.g, color.b, color.a, /* rgba */ 1.0f, 0.0f, /* uv */
         1.0f, 0.0f, 0.0f, /* xyz */ color.r, color.g, color.b, color.a, /* rgba */ 1.0f, 1.0f, /* uv */
         0.0f, 0.0f, 0.0f, /* xyz */ color.r, color.g, color.b, color.a, /* rgba */ 0.0f, 1.0f, /* uv */
         0.0f, 1.0f, 0.0f, /* xyz */ color.r, color.g, color.b, color.a, /* rgba */ 0.0f, 0.0f, /* uv */
    };
    // The order in which the vertices are drawn
    const int rectangleIndex[] = {
        0, 1, 3, // First triangle
        1, 2, 3  // Second traingle
    };

    alignVertex(rectangleVertexData, 4, 9);

    // Aquire the size of each buffer
    int vertexDataSize = sizeof(rectangleVertexData) / sizeof(rectangleVertexData[0]);
    int indexSize = sizeof(rectangleIndex) / sizeof(rectangleIndex[0]);

    // Initialize the render data on shape
    setRenderData(rectangleVertexData, vertexDataSize, rectangleIndex, indexSize);
}

Color Sprite::getColor() {
    return color;
}

void Sprite::setColor(Color color) {
    this->color = color;
    updateRenderData(this->color);
}

void Sprite::addTexture(string path) {
    Frame tex;
    tex.textureID = Loader::loadTexture(path);
    if (tex.textureID == 0) return;
    tex.uvCoords[0] = { 1.0f, 1.0f };
    tex.uvCoords[1] = { 1.0f, 0.0f };
    tex.uvCoords[2] = { 0.0f, 0.0f };
    tex.uvCoords[3] = { 0.0f, 1.0f };
    textures.push_back(tex);
}

void Sprite::draw() {
    if (!textures.empty()) {
        Renderer::bindTexture(textures[0].textureID);
    }
    internalDraw();
}