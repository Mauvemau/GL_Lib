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

bool VirtualScreen::resize(int setWidth, int setHeight) {
    if (setWidth <= 0 || setHeight <= 0) {
        return false;
    }

    if (setWidth == this->width && setHeight == this->height) {
        return true;
    }

    if (!Renderer::resizeDynamicTexture(textureId, setWidth, setHeight)) {
        return false;
    }

    this->width = setWidth;
    this->height = setHeight;
    return true;
}

int VirtualScreen::getWidth() const {
    return width;
}

int VirtualScreen::getHeight() const {
    return height;
}