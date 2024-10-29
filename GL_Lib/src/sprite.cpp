#include "sprite.h"
#include "loader.h"

using namespace gllib;
using namespace std;

Sprite::Sprite(Vector3 translation, Vector3 rotation, Vector3 scale, Color color) :
    Shape(translation, rotation, scale) {
    this->color = color;
    currentTexture = 0;
    updateRenderData(this->color);
    cout << "Created rectangle.\n";
}

Sprite::Sprite(Transform transform, Color color) :
    Shape(transform) {
    this->color = color;
    currentTexture = 0;
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

void Sprite::addTexture(unsigned int textureID) {
    Frame tex;
    tex.textureID = textureID;
    if (tex.textureID == 0) return;
    tex.uvCoords[0] = { 1.0f, 1.0f };
    tex.uvCoords[1] = { 1.0f, 0.0f };
    tex.uvCoords[2] = { 0.0f, 0.0f };
    tex.uvCoords[3] = { 0.0f, 1.0f };
    textures.push_back(tex);
    currentTexture = textures.size() - 1;
}

void Sprite::addTexture(string path) {
    unsigned int texID = Loader::loadTexture(path);
    addTexture(texID);
}

void Sprite::addTexture(unsigned int textureID, int offsetX, int offsetY, int width, int height) {
    Frame tex;
    tex.textureID = textureID;
    if (tex.textureID == 0) return;
    int textureWidth, textureHeight;
    Renderer::getTextureSize(tex.textureID, &textureWidth, &textureHeight);
    
    float uMin = static_cast<float>(offsetX) / textureWidth;
    float vMin = static_cast<float>(offsetY) / textureHeight;
    float uMax = static_cast<float>(offsetX + width) / textureWidth;
    float vMax = static_cast<float>(offsetY + height) / textureHeight;

    tex.uvCoords[0] = { uMax, vMax };
    tex.uvCoords[1] = { uMax, vMin };
    tex.uvCoords[2] = { uMin, vMin };
    tex.uvCoords[3] = { uMin, vMax };

    textures.push_back(tex);
    currentTexture = textures.size() - 1;
}

void Sprite::draw() {
    if (!textures.empty()) {
        Renderer::bindTexture(textures[currentTexture].textureID);
    }
    internalDraw();
}