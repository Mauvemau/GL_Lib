#include "virtual_screen.h"

#include "renderer.h"

#include <iostream>

using namespace gllib;

VirtualScreen::VirtualScreen(int width, int height) : width(width), height(height) {
    textureId = Renderer::createDynamicTexture(width, height);
    if (textureId == 0) {
        throw std::runtime_error("Could not create VirtualScreen texture.");
    }
}

VirtualScreen::~VirtualScreen() {
    Renderer::destroyTexture(textureId);
}

bool VirtualScreen::update(const unsigned char* rgbaPixels, std::size_t byteCount) {
    return Renderer::updateDynamicTexture(textureId, width, height, rgbaPixels, byteCount);
}

unsigned int VirtualScreen::getTextureId() const {
    return textureId;
}

int VirtualScreen::getWidth() const {
    return width;
}

int VirtualScreen::getHeight() const {
    return height;
}