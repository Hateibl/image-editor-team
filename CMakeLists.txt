cmake_minimum_required(VERSION 3.14)
project(ImageEditor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Находим OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Поддержка многопоточности (std::async / std::thread)
find_package(Threads REQUIRED)

# Основная библиотека
add_library(image_editor_lib
    src/image_processor.cpp
    src/filters.cpp
)
target_include_directories(image_editor_lib PUBLIC src)
target_link_libraries(image_editor_lib PUBLIC ${OpenCV_LIBS} Threads::Threads)

# Старые версии GCC (< 9) требуют явной линковки libstdc++fs для <filesystem>
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)
    target_link_libraries(image_editor_lib PUBLIC stdc++fs)
endif()

# Исполняемый файл
add_executable(image_editor src/main.cpp)
target_link_libraries(image_editor PRIVATE image_editor_lib)

# Тесты (опционально, включаются по умолчанию, если найден GTest)
option(BUILD_TESTS "Собирать unit-тесты" ON)

if(BUILD_TESTS)
    find_package(GTest QUIET)
    if(GTest_FOUND OR GTEST_FOUND)
        enable_testing()
        include_directories(${GTEST_INCLUDE_DIRS})

        add_executable(unit_tests tests/test_filters.cpp)
        target_link_libraries(unit_tests PRIVATE image_editor_lib)

        if(TARGET GTest::gtest)
            target_link_libraries(unit_tests PRIVATE GTest::gtest)
        else()
            target_link_libraries(unit_tests PRIVATE ${GTEST_BOTH_LIBRARIES})
        endif()

        add_test(NAME unit_tests COMMAND unit_tests)
    else()
        message(WARNING "GTest не найден — тесты собираться не будут. Установите libgtest-dev или отключите BUILD_TESTS.")
    endif()
endif()
