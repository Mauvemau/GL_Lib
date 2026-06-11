#pragma once

#include "../deps.h"
#include "../transform.h"

namespace gllib {

    class DLLExport Light{
    protected:
        Color color;
        bool active;

    public:
        Light();
        Light(Color color);
        virtual ~Light();

        void setActive(bool active);
        void setColor(Color color);

        virtual Color getColor() const;
        bool isActive() const;
    };

}