#include "filters.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

namespace filters {

cv::Mat grayscale(const cv::Mat& input) {
    cv::Mat gray;
    cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
    cv::cvtColor(gray, gray, cv::COLOR_GRAY2BGR);
    return gray;
}

cv::Mat blur(const cv::Mat& input, int radius) {
    cv::Mat result;
    cv::GaussianBlur(input, result, cv::Size(radius, radius), 0);
    return result;
}

cv::Mat sepia(const cv::Mat& input) {
    cv::Mat sepiaMat = input.clone();
    for (int y = 0; y < sepiaMat.rows; ++y) {
        for (int x = 0; x < sepiaMat.cols; ++x) {
            cv::Vec3b& pixel = sepiaMat.at<cv::Vec3b>(y, x);
            uchar b = pixel[0], g = pixel[1], r = pixel[2];
            pixel[0] = cv::saturate_cast<uchar>(0.272 * r + 0.534 * g + 0.131 * b);
            pixel[1] = cv::saturate_cast<uchar>(0.349 * r + 0.686 * g + 0.168 * b);
            pixel[2] = cv::saturate_cast<uchar>(0.393 * r + 0.769 * g + 0.189 * b);
        }
    }
    return sepiaMat;
}

cv::Mat rotate(const cv::Mat& input, int angle) {
    if (angle % 360 == 0) return input.clone();
    cv::Mat rotated;
    cv::rotate(input, rotated, angle == 90 ? cv::ROTATE_90_CLOCKWISE :
                                     angle == 180 ? cv::ROTATE_180 :
                                     cv::ROTATE_90_COUNTERCLOCKWISE);
    return rotated;
}

cv::Mat resize(const cv::Mat& input, double scale) {
    cv::Mat resized;
    cv::resize(input, resized, cv::Size(), scale, scale, cv::INTER_LINEAR);
    return resized;
}

} 