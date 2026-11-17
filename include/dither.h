#ifndef DITHER_H
#define DITHER_H

#include <opencv2/opencv.hpp>

enum class dither_type {
    ordered_bayer_2x2,
    ordered_bayer_4x4,
    atkinson,
    floyd_steinberg,
    sierra_lite,
    stucki,
    none
};

namespace Dither
{
    void filter(const cv::Mat& input, cv::Mat& output, dither_type type);
}

#endif // !DITHER_H
