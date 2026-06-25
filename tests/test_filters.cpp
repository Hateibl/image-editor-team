#include <gtest/gtest.h>
#include "filters.h"

TEST(FiltersTest, Grayscale) {
    cv::Mat input = cv::Mat::zeros(10, 10, CV_8UC3);
    cv::circle(input, cv::Point(5,5), 3, cv::Scalar(0,0,255), -1); 
    cv::Mat result = filters::grayscale(input);
    cv::Vec3b p = result.at<cv::Vec3b>(5,5);
    EXPECT_NEAR(p[0], p[1], 1);
    EXPECT_NEAR(p[1], p[2], 1);
}

TEST(FiltersTest, Blur) {
    cv::Mat input = cv::Mat::ones(10,10,CV_8UC3) * 100;
    cv::Mat result = filters::blur(input, 3);
    EXPECT_EQ(input.at<cv::Vec3b>(0,0)[0], result.at<cv::Vec3b>(0,0)[0]);
}

TEST(FiltersTest, Rotate) {
    cv::Mat input = cv::Mat::zeros(10,20,CV_8UC3);
    cv::rectangle(input, cv::Rect(0,0,5,10), cv::Scalar(255,255,255), -1);
    cv::Mat rotated = filters::rotate(input, 90);
    EXPECT_EQ(rotated.rows, input.cols);
    EXPECT_EQ(rotated.cols, input.rows);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}