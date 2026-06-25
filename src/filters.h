#pragma once
#include <opencv2/opencv.hpp>

namespace filters {

cv::Mat grayscale(const cv::Mat& input);

cv::Mat blur(const cv::Mat& input, int radius = 5);

cv::Mat sepia(const cv::Mat& input);

cv::Mat rotate(const cv::Mat& input, int angle);

cv::Mat resize(const cv::Mat& input, double scale);

} 