#include "video_asset.h"

#include <iostream>
#include <iomanip>

using namespace gllib;
using namespace std;

void VideoAsset::printInfo() {
    const double frameRate = frameRateDenominator != 0 ? static_cast<double>(frameRateNumerator) / frameRateDenominator : 0.0;

    std::cout << "Video information\n"
    << "  Path: " << path << '\n'
    << "  Resolution: " << width << " x " << height << '\n'
    << "  Duration: " << std::fixed << std::setprecision(2)
    << durationInSeconds << " seconds\n"
    << "  Frame rate: " << frameRateNumerator << '/'
    << frameRateDenominator << " ("
    << std::setprecision(3) << frameRate << " FPS)\n"
    << "  Codec: " << codecName << '\n';
}
