#pragma once

#include "../deps.h"
#include "../transform.h"

namespace gllib {

    class DLLExport Light{
    protected:
        Color color;

    public:
        Light();
        Light(Color color);
        virtual ~Light();

        void setColor(Color color);

        virtual Color getColor() const;
    };

}