#include "dither.h"

namespace
{
    // clang-format off
    const cv::Mat ORDERED_BAYER_2X2 = (cv::Mat_<uchar>(2, 2) << 0, 128,
                                                                192, 64
    );

    const cv::Mat ORDERED_BAYER_4X4 = (cv::Mat_<uchar>(4, 4) << 0, 128, 32, 160,
                                                                192, 64, 224, 96,
                                                                48, 176, 16, 144,
                                                                240, 112, 208, 80
    );

    const cv::Mat FLOYD_STEINBERG_ERROR = (cv::Mat_<uchar>(2, 3) << 0, 0, 112,
                                                                    48, 80, 16
    );
    // clang-format on

    void ordered_bayer_2x2(const cv::Mat& input, cv::Mat& output)
    {
        // TODO: SIMD AVX2
        cv::Mat grayInput;
        cv::cvtColor(input, grayInput, cv::COLOR_BGR2GRAY);
        output.create(input.rows, input.cols, CV_8U);
        const uchar* grayData = grayInput.data;
        uchar* outputData = output.data;

        for (int row = 0; row < input.rows; ++row) {
            for (int col = 0; col < input.cols; ++col) {
                outputData[input.cols * row + col]
                  = 255
                    * (grayData[input.cols * row + col]
                       > ORDERED_BAYER_2X2.at<uchar>(col % 2, row % 2));
            }
        }
    }

    void ordered_bayer_4x4(const cv::Mat& input, cv::Mat& output)
    {
        // TODO: SIMD AVX2
        cv::Mat grayInput;
        cv::cvtColor(input, grayInput, cv::COLOR_BGR2GRAY);
        output.create(input.rows, input.cols, CV_8U);
        const uchar* grayData = grayInput.data;
        uchar* outputData = output.data;

        for (int row = 0; row < input.rows; ++row) {
            for (int col = 0; col < input.cols; ++col) {
                outputData[input.cols * row + col]
                  = 255
                    * (grayData[input.cols * row + col]
                       > ORDERED_BAYER_4X4.at<uchar>(col % 4, row % 4));
            }
        }
    }

    void atkinson(const cv::Mat& input, cv::Mat& output)
    {
        cv::Mat grayInput, floatInput;
        cv::cvtColor(input, grayInput, cv::COLOR_BGR2GRAY);
        grayInput.convertTo(floatInput, CV_32FC1);

        for (int row = 0; row < input.rows; ++row) {
            for (int col = 0; col < input.cols; ++col) {
                float currentValue = floatInput.at<float>(row, col);
                float nextValue = 255.f * (currentValue > 127.f);
                floatInput.at<float>(row, col) = nextValue;

                float error = currentValue - nextValue;

                if (col + 1 < floatInput.cols) {
                    floatInput.at<float>(row, col + 1) += error * 0.125f;
                }
                if (col + 2 < floatInput.cols) {
                    floatInput.at<float>(row, col + 2) += error * 0.125f;
                }
                if (row + 1 < floatInput.rows && col - 1 >= 0) {
                    floatInput.at<float>(row + 1, col - 1) += error * 0.125f;
                }
                if (row + 1 < floatInput.rows) {
                    floatInput.at<float>(row + 1, col) += error * 0.125f;
                }
                if (row + 1 < floatInput.rows && col + 1 < floatInput.cols) {
                    floatInput.at<float>(row + 1, col + 1) += error * 0.125f;
                }
                if (row + 1 < floatInput.rows && col + 2 < floatInput.cols) {
                    floatInput.at<float>(row + 1, col + 2) += error * 0.125f;
                }
            }
        }
        floatInput.convertTo(output, CV_8UC1);
    }

    void floyd_steinberg(const cv::Mat& input, cv::Mat& output)
    {
        cv::Mat grayInput, floatInput;
        cv::cvtColor(input, grayInput, cv::COLOR_BGR2GRAY);
        grayInput.convertTo(floatInput, CV_32FC1);

        for (int row = 0; row < input.rows; ++row) {
            for (int col = 0; col < input.cols; ++col) {
                float currentValue = floatInput.at<float>(row, col);
                float nextValue = 255.f * (currentValue > 127.f);
                floatInput.at<float>(row, col) = nextValue;

                float error = currentValue - nextValue;

                if (col + 1 < floatInput.cols) {
                    floatInput.at<float>(row, col + 1) += error * 0.4375f;
                }
                if (row + 1 < floatInput.rows && col - 1 >= 0) {
                    floatInput.at<float>(row + 1, col - 1) += error * 0.1875f;
                }
                if (row + 1 < floatInput.rows) {
                    floatInput.at<float>(row + 1, col) += error * 0.3125f;
                }
                if (row + 1 < floatInput.rows && col + 1 < floatInput.cols) {
                    floatInput.at<float>(row + 1, col + 1) += error * 0.0625f;
                }
            }
        }
        floatInput.convertTo(output, CV_8UC1);
    }

    void sierra_lite(const cv::Mat& input, cv::Mat& output)
    {
        cv::Mat grayInput, floatInput;
        cv::cvtColor(input, grayInput, cv::COLOR_BGR2GRAY);
        grayInput.convertTo(floatInput, CV_32FC1);

        for (int row = 0; row < input.rows; ++row) {
            for (int col = 0; col < input.cols; ++col) {
                float currentValue = floatInput.at<float>(row, col);
                float nextValue = 255.f * (currentValue > 127.f);
                floatInput.at<float>(row, col) = nextValue;

                float error = currentValue - nextValue;

                if (col + 1 < floatInput.cols) {
                    floatInput.at<float>(row, col + 1) += error * 0.5f;
                }
                if (row + 1 < floatInput.rows && col - 1 >= 0) {
                    floatInput.at<float>(row + 1, col - 1) += error * 0.25f;
                }
                if (row + 1 < floatInput.rows) {
                    floatInput.at<float>(row + 1, col) += error * 0.25f;
                }
            }
        }
        floatInput.convertTo(output, CV_8UC1);
    }

    void stucki(const cv::Mat& input, cv::Mat& output)
    {
        cv::Mat grayInput, floatInput;
        cv::cvtColor(input, grayInput, cv::COLOR_BGR2GRAY);
        grayInput.convertTo(floatInput, CV_32FC1);

        for (int row = 0; row < input.rows; ++row) {
            for (int col = 0; col < input.cols; ++col) {
                float currentValue = floatInput.at<float>(row, col);
                float nextValue = 255.f * (currentValue > 127.f);
                floatInput.at<float>(row, col) = nextValue;

                float error = currentValue - nextValue;

                if (col + 1 < floatInput.cols) {
                    floatInput.at<float>(row, col + 1) += error * 8.f / 42.f;
                }
                if (col + 2 < floatInput.cols) {
                    floatInput.at<float>(row, col + 2) += error * 4.f / 42.f;
                }
                if (row + 1 < floatInput.rows && col - 2 >= 0) {
                    floatInput.at<float>(row + 1, col - 2) += error * 2.f / 42.f;
                }
                if (row + 1 < floatInput.rows && col - 1 >= 0) {
                    floatInput.at<float>(row + 1, col - 1) += error * 4.f / 42.f;
                }
                if (row + 1 < floatInput.rows) {
                    floatInput.at<float>(row + 1, col) += error * 8.f / 42.f;
                }
                if (row + 1 < floatInput.rows && col + 1 < floatInput.cols) {
                    floatInput.at<float>(row + 1, col + 1) += error * 4.f / 42.f;
                }
                if (row + 1 < floatInput.rows && col + 2 < floatInput.cols) {
                    floatInput.at<float>(row + 1, col + 2) += error * 2.f / 42.f;
                }
                if (row + 2 < floatInput.rows && col - 2 >= 0) {
                    floatInput.at<float>(row + 2, col - 2) += error * 1.f / 42.f;
                }
                if (row + 2 < floatInput.rows && col - 1 >= 0) {
                    floatInput.at<float>(row + 2, col - 1) += error * 2.f / 42.f;
                }
                if (row + 2 < floatInput.rows) {
                    floatInput.at<float>(row + 2, col) += error * 4.f / 42.f;
                }
                if (row + 2 < floatInput.rows && col + 1 < floatInput.cols) {
                    floatInput.at<float>(row + 2, col + 1) += error * 2.f / 42.f;
                }
                if (row + 2 < floatInput.rows && col + 2 < floatInput.cols) {
                    floatInput.at<float>(row + 2, col + 2) += error * 1.f / 42.f;
                }
            }
        }
        floatInput.convertTo(output, CV_8UC1);
    }
}

void Dither::filter(const cv::Mat& input, cv::Mat& output, dither_type type)
{
    switch (type) {
        case dither_type::ordered_bayer_2x2: {
            ordered_bayer_2x2(input, output);
            break;
        }
        case dither_type::ordered_bayer_4x4: {
            ordered_bayer_4x4(input, output);
            break;
        }
        case dither_type::atkinson: {
            atkinson(input, output);
            break;
        }
        case dither_type::floyd_steinberg: {
            floyd_steinberg(input, output);
            break;
        }
        case dither_type::sierra_lite: {
            sierra_lite(input, output);
            break;
        }
        case dither_type::stucki: {
            stucki(input, output);
            break;
        }
        default: {
            output = input.clone();
            break;
        }
    }
}
