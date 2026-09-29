#pragma once

#include "deps.h"

#include <cstddef>

namespace gllib {
    class VirtualScreen {
    private:
        unsigned int textureId = 0;
        int width = 0, height = 0;

    public:
        VirtualScreen(int width, int height);
        VirtualScreen& operator=(const VirtualScreen&) = delete;
        ~VirtualScreen();

        bool update(const unsigned char* rgbaPixels, std::size_t rgbaByteCount);

        unsigned int getTextureId() const;

        int getWidth() const;
        int getHeight() const;
    };
}