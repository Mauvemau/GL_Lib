#pragma once

#include "deps.h"
#include <string>

namespace gllib {
    class DLLExport VideoAsset {
        public:
        std::string path;
        int width = 0, height = 0;
        double durationInSeconds = 0.0;
        int frameRateNumerator = 0, frameRateDenominator = 1;
        std::string codecName;

        void printInfo();
    };
}