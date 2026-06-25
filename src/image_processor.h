#pragma once
#include <string>
#include <vector>
#include <future>
#include <filesystem>
#include <opencv2/opencv.hpp>

namespace fs = std::filesystem;

struct FilterPipeline {
    bool use_grayscale = false;
    bool use_blur = false;
    int blur_radius = 5;
    bool use_sepia = false;
    bool use_rotate = false;
    int rotate_angle = 90;
    bool use_resize = false;
    double resize_scale = 1.0;
};

class ImageProcessor {
public:
    ImageProcessor(const std::string& inputDir,
                   const std::string& outputDir,
                   const FilterPipeline& pipeline);

    void run();

private:
    void processOne(const fs::path& inputPath, const fs::path& outputPath);

    fs::path inputDir_;
    fs::path outputDir_;
    FilterPipeline pipeline_;
};