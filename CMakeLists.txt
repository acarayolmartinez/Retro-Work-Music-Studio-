cmake_minimum_required(VERSION 3.10)

project(RetroWorksMusicStudio
    VERSION 0.2.0
    LANGUAGES CXX
)

# RMS requires C++11 across every application module.
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(RMS_BUILD_TESTS "Build RMS unit tests" OFF)

add_library(rms_core
    src/core/application.cpp
)

target_include_directories(rms_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(rms_core PUBLIC cxx_std_11)

add_executable(rms
    src/main.cpp
)

target_link_libraries(rms PRIVATE rms_core)

if(MSVC)
    target_compile_options(rms_core PRIVATE /W4)
    target_compile_options(rms PRIVATE /W4)
else()
    target_compile_options(rms_core PRIVATE -Wall -Wextra -pedantic)
    target_compile_options(rms PRIVATE -Wall -Wextra -pedantic)
endif()

if(RMS_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
