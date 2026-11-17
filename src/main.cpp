#include <opencv2/opencv.hpp>

#include "dither.h"
#include "io_utils.h"

namespace
{
    dither_type Type = dither_type::none;

    void event_loop(cv::VideoCapture& capture)
    {
        bool isRolling = true;
        while (isRolling) {
            cv::Mat frame, dithered_frame;
            capture >> frame;
            cv::flip(frame, frame, 1);

            Dither::filter(frame, dithered_frame, Type);
            cv::imshow("app", dithered_frame);

            switch (cv::waitKey(16)) {
                case 'q': {
                    isRolling = false;
                }
                case 'p': {
                    cv::imwrite("screenshots/screenshot.png", dithered_frame);
                    std::cout << "printed a new screenshot\n";
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
    }
}

int main()
{
    if (!io_utils::is_folder_exists("screenshots/")) {
        io_utils::create_folder("screenshots/");
    }
    cv::VideoCapture capture(0);
    event_loop(capture);
    return 0;
}
