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
option(RMS_ENABLE_PORTAUDIO "Enable real audio output through PortAudio" OFF)

add_library(rms_core
    src/core/application.cpp
    src/audio/audio_buffer.cpp
    src/audio/audio_file_reader.cpp
    src/audio/audio_analyzer.cpp
    src/audio/transient_detector.cpp
    src/audio/beat_interval_analyzer.cpp
    src/audio/tempo_estimator.cpp
    src/audio/key_detector.cpp
    src/audio/pitch_detector.cpp
    src/audio/melody_tracker.cpp
    src/audio/note_event.cpp
    src/audio/musical_timeline.cpp
    src/audio/audio_analysis_pipeline.cpp
    src/audio/waveform_data.cpp
    src/audio/waveform_viewport.cpp
    src/audio/audio_playback.cpp
    src/audio/audio_playback_controller.cpp
    src/audio/audio_project.cpp
    src/audio/audio_project_file.cpp
    src/audio/audio_project_manager.cpp
    src/audio/audio_import_manager.cpp
    src/audio/audio_import_config.cpp
    src/audio/audio_import_format.cpp
    src/audio/audio_import_report.cpp
    src/audio/audio_import_history.cpp
    src/audio/audio_import_history_csv_exporter.cpp
    src/audio/audio_import_history_store.cpp
    src/audio/audio_output.cpp
    src/audio/audio_device_manager.cpp
    src/audio/audio_settings.cpp
    src/audio/audio_settings_store.cpp
    src/audio/audio_settings_manager.cpp
)

target_include_directories(rms_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(rms_core PUBLIC cxx_std_11)

if(RMS_ENABLE_PORTAUDIO)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(PORTAUDIO REQUIRED IMPORTED_TARGET portaudio-2.0)
    target_compile_definitions(rms_core PUBLIC RMS_ENABLE_PORTAUDIO)
    target_link_libraries(rms_core PUBLIC PkgConfig::PORTAUDIO)
endif()

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
