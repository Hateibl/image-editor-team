#include "image_processor.h"
#include "filters.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <future>

ImageProcessor::ImageProcessor(const std::string& inputDir,
                               const std::string& outputDir,
                               const FilterPipeline& pipeline)
    : inputDir_(inputDir), outputDir_(outputDir), pipeline_(pipeline) {}

void ImageProcessor::processOne(const fs::path& inputPath, const fs::path& outputPath) {
    cv::Mat img = cv::imread(inputPath.string(), cv::IMREAD_COLOR);
    if (img.empty()) {
        std::cerr << "Ошибка чтения: " << inputPath << std::endl;
        return;
    }

    if (pipeline_.use_grayscale) {
        img = filters::grayscale(img);
    }
    if (pipeline_.use_blur) {
        img = filters::blur(img, pipeline_.blur_radius);
    }
    if (pipeline_.use_sepia) {
        img = filters::sepia(img);
    }
    if (pipeline_.use_rotate) {
        img = filters::rotate(img, pipeline_.rotate_angle);
    }
    if (pipeline_.use_resize) {
        img = filters::resize(img, pipeline_.resize_scale);
    }

    if (!cv::imwrite(outputPath.string(), img)) {
        std::cerr << "Ошибка записи: " << outputPath << std::endl;
    }
}

void ImageProcessor::run() {
    fs::create_directories(outputDir_);
    std::vector<fs::path> imageFiles;
    for (const auto& entry : fs::directory_iterator(inputDir_)) {
        if (entry.is_regular_file()) {
            auto ext = entry.path().extension().string();
            if (ext == ".jpg" || ext == ".jpeg" || ext == ".png" ||
                ext == ".bmp" || ext == ".tiff" || ext == ".tif") {
                imageFiles.push_back(entry.path());
            }
        }
    }

    if (imageFiles.empty()) {
        std::cout << "Нет изображений для обработки." << std::endl;
        return;
    }

    std::cout << "Начинаем обработку " << imageFiles.size() << " файлов...\n";
    unsigned int num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0) num_threads = 4;
    std::cout << "Используем до " << num_threads << " потоков.\n";

    std::vector<std::future<void>> futures;
    for (const auto& inPath : imageFiles) {
        fs::path outPath = outputDir_ / inPath.filename();
        futures.emplace_back(std::async(std::launch::async, [this, inPath, outPath]() {
            this->processOne(inPath, outPath);
        }));
    }
    for (auto& f : futures) {
        f.wait();  
    }

    std::cout << "Обработка завершена. Результаты в: " << outputDir_ << std::endl;
}