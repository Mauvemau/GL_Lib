#pragma once

#include "deps.h"
#include "video_asset.h"

namespace gllib {
    class DLLExport VideoLoader {
        public:
        static VideoAsset load(const std::string& path);
    };
}