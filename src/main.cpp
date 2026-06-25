#include "image_processor.h"
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>

namespace {

void printUsage(const char* progName) {
    std::cout <<
        "Использование: " << progName << " [опции]\n"
        "\n"
        "Опции:\n"
        "  --input <папка>      Папка с исходными изображениями (по умолчанию: ./input)\n"
        "  --output <папка>     Папка для результатов (по умолчанию: ./output)\n"
        "  --grayscale          Применить оттенки серого\n"
        "  --blur [радиус]      Применить размытие (по умолчанию радиус: 5)\n"
        "  --sepia              Применить сепию\n"
        "  --rotate [угол]      Повернуть изображение (90, 180, 270; по умолчанию: 90)\n"
        "  --resize <масштаб>   Изменить размер (например, 1.5 = увеличить на 50%)\n"
        "  -h, --help           Показать эту справку\n";
}
bool looksLikeNumber(const char* s) {
    if (s == nullptr || *s == '\0') return false;
    char* end = nullptr;
    std::strtod(s, &end);
    return end != s && *end == '\0';
}

} 
int main(int argc, char* argv[]) {
    std::string inputDir = "./input";
    std::string outputDir = "./output";
    FilterPipeline pipeline;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            printUsage(argv[0]);
            return 0;
        } else if (arg == "--input") {
            if (i + 1 >= argc) {
                std::cerr << "Ошибка: --input требует значение (путь к папке)\n";
                return 1;
            }
            inputDir = argv[++i];
        } else if (arg == "--output") {
            if (i + 1 >= argc) {
                std::cerr << "Ошибка: --output требует значение (путь к папке)\n";
                return 1;
            }
            outputDir = argv[++i];
        } else if (arg == "--grayscale") {
            pipeline.use_grayscale = true;
        } else if (arg == "--blur") {
            pipeline.use_blur = true;
            if (i + 1 < argc && looksLikeNumber(argv[i + 1])) {
                pipeline.blur_radius = std::atoi(argv[++i]);
                if (pipeline.blur_radius % 2 == 0) {
                    ++pipeline.blur_radius; 
                }
            }
        } else if (arg == "--sepia") {
            pipeline.use_sepia = true;
        } else if (arg == "--rotate") {
            pipeline.use_rotate = true;
            if (i + 1 < argc && looksLikeNumber(argv[i + 1])) {
                pipeline.rotate_angle = std::atoi(argv[++i]);
            }
        } else if (arg == "--resize") {
            if (i + 1 >= argc || !looksLikeNumber(argv[i + 1])) {
                std::cerr << "Ошибка: --resize требует числовое значение масштаба\n";
                return 1;
            }
            pipeline.use_resize = true;
            pipeline.resize_scale = std::atof(argv[++i]);
        } else {
            std::cerr << "Неизвестный аргумент: " << arg << "\n";
            printUsage(argv[0]);
            return 1;
        }
    }

    std::cout << "Входная папка: " << inputDir << "\n";
    std::cout << "Выходная папка: " << outputDir << "\n";

    ImageProcessor processor(inputDir, outputDir, pipeline);
    processor.run();

    return 0;
}
