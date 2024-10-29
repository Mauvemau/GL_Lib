#pragma once
#include <map>
#include <string>

#include "transform.h"
#include "sprite.h"

namespace gllib
{
    class DLLExport animation
    {
    private:
        Sprite sprite;
        
        int currentFrameIndex;
        double durationInSecs;
        double elapsedTime;
        bool mirrorX;
        bool mirrorY;
        
    public:
        animation(gllib::Vector3 position, Vector3 scale, Color color);
        virtual ~animation();

        void update();

        void addFrame(unsigned int textureID, float offsetX, float offsetY, float width, float height, int frameAmount);

        void setSpriteSheet(unsigned int spriteSheetID, int amountColumns, int amountRows, double durationInSecs);

        void setDurationInSecs(double durationInSecs);
        void setCurrentFrame(int frame);
        void setMirrorX(bool mirrorX);
        void setMirrorY(bool mirrorY);
        
        void draw();
    };
}
