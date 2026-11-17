#include <opencv2/opencv.hpp>

#include "dither.h"
#include "utils.h"

namespace
{
    dither_type Type = dither_type::none;
}

int main()
{
    if (!Utils::is_folder_exists("screenshots/")) {
        Utils::create_folder("screenshots/");
    }

    cv::VideoCapture cap(0);
    bool isRolling = true;

    while (isRolling) {
        cv::Mat frame, dithered_frame;
        cap >> frame;
        cv::flip(frame, frame, 1);

        Dither::filter(frame, dithered_frame, Type);
        cv::imshow("app", dithered_frame);

        switch (cv::waitKey(16)) {
            case 'q': {
                isRolling = false;
            }
            case 'p': {
                cv::imwrite("screenshots/screenshot.png", dithered_frame);
                break;
            }
            case '0': {
                Type = dither_type::none;
                break;
            }
            case '1': {
                Type = dither_type::ordered_bayer_2x2;
                break;
            }
            case '2': {
                Type = dither_type::ordered_bayer_4x4;
                break;
            }
            case '3': {
                Type = dither_type::atkinson;
                break;
            }
            case '4': {
                Type = dither_type::floyd_steinberg;
                break;
            }
            case '5': {
                Type = dither_type::sierra_lite;
                break;
            }
            case '6': {
                Type = dither_type::stucki;
                break;
            }
            default: {
                break;
            }
        }
    }
    return 0;
}
