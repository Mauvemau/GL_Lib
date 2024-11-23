#pragma once
#include "../Dependencies/deps.h"
#include "window.h"
#include "../Utils/core.h"
#include "../Utils/Input.h"


namespace gllib
{
    class DLLExport BaseGame
    {
    private:
        LibCore libCore;

        bool initInternal();
        void updateInternal();
        void uninitInternal();

    protected:
        Window* window;
        Input* input;

        unsigned int shaderProgramSolidColor;
        unsigned int shaderProgramTexture;

        virtual void init()
        {
        };

        virtual void update()
        {
        };

        virtual void uninit()
        {
        };

    public:
        BaseGame();
        virtual ~BaseGame();

        void start();
    };
}
