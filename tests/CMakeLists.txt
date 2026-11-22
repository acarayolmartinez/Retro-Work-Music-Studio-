add_executable(rms_test_application
    test_application.cpp
)

target_link_libraries(rms_test_application
    PRIVATE
        rms_core
)

if(MSVC)
    target_compile_options(rms_test_application PRIVATE /W4)
else()
    target_compile_options(rms_test_application PRIVATE -Wall -Wextra -pedantic)
endif()

add_test(
    NAME rms.application.name
    COMMAND rms_test_application
)

add_executable(rms_test_audio_buffer
    test_audio_buffer.cpp
)

target_link_libraries(rms_test_audio_buffer
    PRIVATE
        rms_core
)

if(MSVC)
    target_compile_options(rms_test_audio_buffer PRIVATE /W4)
else()
    target_compile_options(rms_test_audio_buffer PRIVATE -Wall -Wextra -pedantic)
endif()

add_test(
    NAME rms.audio.buffer
    COMMAND rms_test_audio_buffer
)

add_executable(rms_test_audio_file_reader
    test_audio_file_reader.cpp
)

target_link_libraries(rms_test_audio_file_reader
    PRIVATE
        rms_core
)

if(MSVC)
    target_compile_options(rms_test_audio_file_reader PRIVATE /W4)
else()
    target_compile_options(rms_test_audio_file_reader PRIVATE -Wall -Wextra -pedantic)
endif()

add_test(
    NAME rms.audio.file_reader
    COMMAND rms_test_audio_file_reader
)

add_executable(rms_test_audio_analyzer
    test_audio_analyzer.cpp
)

target_link_libraries(rms_test_audio_analyzer
    PRIVATE
        rms_core
)

if(MSVC)
    target_compile_options(rms_test_audio_analyzer PRIVATE /W4)
else()
    target_compile_options(rms_test_audio_analyzer PRIVATE -Wall -Wextra -pedantic)
endif()

add_test(
    NAME rms.audio.analyzer
    COMMAND rms_test_audio_analyzer
)

add_executable(rms_test_transient_detector
    test_transient_detector.cpp
)

target_link_libraries(rms_test_transient_detector
    PRIVATE
        rms_core
)

if(MSVC)
    target_compile_options(rms_test_transient_detector PRIVATE /W4)
else()
    target_compile_options(rms_test_transient_detector PRIVATE -Wall -Wextra -pedantic)
endif()

add_test(
    NAME rms.audio.transient_detector
    COMMAND rms_test_transient_detector
)

add_executable(rms_test_beat_interval_analyzer
    test_beat_interval_analyzer.cpp
)

target_link_libraries(rms_test_beat_interval_analyzer
    PRIVATE
        rms_core
)

if(MSVC)
    target_compile_options(rms_test_beat_interval_analyzer PRIVATE /W4)
else()
    target_compile_options(rms_test_beat_interval_analyzer PRIVATE -Wall -Wextra -pedantic)
endif()

add_test(
    NAME rms.audio.beat_interval_analyzer
    COMMAND rms_test_beat_interval_analyzer
)

add_executable(rms_test_tempo_estimator
    test_tempo_estimator.cpp
)

target_link_libraries(rms_test_tempo_estimator
    PRIVATE
        rms_core
)

if(MSVC)
    target_compile_options(rms_test_tempo_estimator PRIVATE /W4)
else()
    target_compile_options(rms_test_tempo_estimator PRIVATE -Wall -Wextra -pedantic)
endif()

add_test(
    NAME rms.audio.tempo_estimator
    COMMAND rms_test_tempo_estimator
)

add_executable(rms_test_key_detector
    test_key_detector.cpp
)

target_link_libraries(rms_test_key_detector
    PRIVATE
        rms_core
)

if(MSVC)
    target_compile_options(rms_test_key_detector PRIVATE /W4)
else()
    target_compile_options(rms_test_key_detector PRIVATE -Wall -Wextra -pedantic)
endif()

add_test(
    NAME rms.audio.key_detector
    COMMAND rms_test_key_detector
)

add_executable(rms_test_pitch_detector
    test_pitch_detector.cpp
)

target_link_libraries(rms_test_pitch_detector
    PRIVATE
        rms_core
)

if(MSVC)
    target_compile_options(rms_test_pitch_detector PRIVATE /W4)
else()
    target_compile_options(rms_test_pitch_detector PRIVATE -Wall -Wextra -pedantic)
endif()

add_test(
    NAME rms.audio.pitch_detector
    COMMAND rms_test_pitch_detector
)

add_executable(rms_test_melody_tracker
    test_melody_tracker.cpp
)

target_link_libraries(rms_test_melody_tracker
    PRIVATE
        rms_core
)

if(MSVC)
    target_compile_options(rms_test_melody_tracker PRIVATE /W4)
else()
    target_compile_options(rms_test_melody_tracker PRIVATE -Wall -Wextra -pedantic)
endif()

add_test(
    NAME rms.audio.melody_tracker
    COMMAND rms_test_melody_tracker
)

add_executable(rms_test_note_event
    test_note_event.cpp
)

target_link_libraries(rms_test_note_event
    PRIVATE
        rms_core
)

if(MSVC)
    target_compile_options(rms_test_note_event PRIVATE /W4)
else()
    target_compile_options(rms_test_note_event PRIVATE -Wall -Wextra -pedantic)
endif()

add_test(
    NAME rms.audio.note_event
    COMMAND rms_test_note_event
)

add_executable(rms_test_musical_timeline
    test_musical_timeline.cpp
)

target_link_libraries(rms_test_musical_timeline
    PRIVATE
        rms_core
)

if(MSVC)
    target_compile_options(rms_test_musical_timeline PRIVATE /W4)
else()
    target_compile_options(rms_test_musical_timeline PRIVATE -Wall -Wextra -pedantic)
endif()

add_test(
    NAME rms.audio.musical_timeline
    COMMAND rms_test_musical_timeline
)

add_executable(rms_test_audio_analysis_pipeline
    test_audio_analysis_pipeline.cpp
)

target_link_libraries(rms_test_audio_analysis_pipeline
    PRIVATE
        rms_core
)

if(MSVC)
    target_compile_options(rms_test_audio_analysis_pipeline PRIVATE /W4)
else()
    target_compile_options(rms_test_audio_analysis_pipeline PRIVATE -Wall -Wextra -pedantic)
endif()

add_test(
    NAME rms.audio.analysis_pipeline
    COMMAND rms_test_audio_analysis_pipeline
)

add_executable(rms_test_waveform_data
    test_waveform_data.cpp
)

target_link_libraries(rms_test_waveform_data
    PRIVATE
        rms_core
)

if(MSVC)
    target_compile_options(rms_test_waveform_data PRIVATE /W4)
else()
    target_compile_options(rms_test_waveform_data PRIVATE -Wall -Wextra -pedantic)
endif()

add_test(
    NAME rms.audio.waveform_data
    COMMAND rms_test_waveform_data
)

add_executable(rms_test_waveform_viewport
    test_waveform_viewport.cpp
)

target_link_libraries(rms_test_waveform_viewport
    PRIVATE
        rms_core
)

if(MSVC)
    target_compile_options(rms_test_waveform_viewport PRIVATE /W4)
else()
    target_compile_options(rms_test_waveform_viewport PRIVATE -Wall -Wextra -pedantic)
endif()

add_test(
    NAME rms.audio.waveform_viewport
    COMMAND rms_test_waveform_viewport
)

add_executable(rms_test_audio_playback
    test_audio_playback.cpp
)

target_link_libraries(rms_test_audio_playback
    PRIVATE
        rms_core
)

if(MSVC)
    target_compile_options(rms_test_audio_playback PRIVATE /W4)
else()
    target_compile_options(rms_test_audio_playback PRIVATE -Wall -Wextra -pedantic)
endif()

add_test(
    NAME rms.audio.playback
    COMMAND rms_test_audio_playback
)

add_executable(rms_test_audio_output
    test_audio_output.cpp
)

target_link_libraries(rms_test_audio_output
    PRIVATE
        rms_core
)

if(MSVC)
    target_compile_options(rms_test_audio_output PRIVATE /W4)
else()
    target_compile_options(rms_test_audio_output PRIVATE -Wall -Wextra -pedantic)
endif()

add_test(
    NAME rms.audio.output
    COMMAND rms_test_audio_output
)

add_executable(rms_test_audio_device_manager
    test_audio_device_manager.cpp
)

target_link_libraries(rms_test_audio_device_manager
    PRIVATE
        rms_core
)

if(MSVC)
    target_compile_options(rms_test_audio_device_manager PRIVATE /W4)
else()
    target_compile_options(rms_test_audio_device_manager PRIVATE -Wall -Wextra -pedantic)
endif()

add_test(
    NAME rms.audio.device_manager
    COMMAND rms_test_audio_device_manager
)

add_executable(rms_test_audio_settings
    test_audio_settings.cpp
)

target_link_libraries(rms_test_audio_settings
    PRIVATE
        rms_core
)

if(MSVC)
    target_compile_options(rms_test_audio_settings PRIVATE /W4)
else()
    target_compile_options(rms_test_audio_settings PRIVATE -Wall -Wextra -pedantic)
endif()

add_test(
    NAME rms.audio.settings
    COMMAND rms_test_audio_settings
)

add_executable(rms_test_audio_settings_store
    test_audio_settings_store.cpp
)

target_link_libraries(rms_test_audio_settings_store
    PRIVATE
        rms_core
)

if(MSVC)
    target_compile_options(rms_test_audio_settings_store PRIVATE /W4)
else()
    target_compile_options(rms_test_audio_settings_store PRIVATE -Wall -Wextra -pedantic)
endif()

add_test(
    NAME rms.audio.settings_store
    COMMAND rms_test_audio_settings_store
)

add_executable(rms_test_audio_settings_manager
    test_audio_settings_manager.cpp
)

target_link_libraries(rms_test_audio_settings_manager
    PRIVATE
        rms_core
)

if(MSVC)
    target_compile_options(rms_test_audio_settings_manager PRIVATE /W4)
else()
    target_compile_options(rms_test_audio_settings_manager PRIVATE -Wall -Wextra -pedantic)
endif()

add_test(
    NAME rms.audio.settings_manager
    COMMAND rms_test_audio_settings_manager
)

add_executable(rms_test_audio_playback_controller
    test_audio_playback_controller.cpp
)

target_link_libraries(rms_test_audio_playback_controller
    PRIVATE
        rms_core
)

if(MSVC)
    target_compile_options(rms_test_audio_playback_controller PRIVATE /W4)
else()
    target_compile_options(rms_test_audio_playback_controller PRIVATE -Wall -Wextra -pedantic)
endif()

add_test(
    NAME rms.audio.playback_controller
    COMMAND rms_test_audio_playback_controller
)

add_executable(rms_test_audio_project
    test_audio_project.cpp
)

target_link_libraries(rms_test_audio_project
    PRIVATE
        rms_core
)

if(MSVC)
    target_compile_options(rms_test_audio_project PRIVATE /W4)
else()
    target_compile_options(rms_test_audio_project PRIVATE -Wall -Wextra -pedantic)
endif()

add_test(
    NAME rms.audio.project
    COMMAND rms_test_audio_project
)

add_executable(rms_test_audio_project_file
    test_audio_project_file.cpp
)

target_link_libraries(rms_test_audio_project_file
    PRIVATE
        rms_core
)

if(MSVC)
    target_compile_options(rms_test_audio_project_file PRIVATE /W4)
else()
    target_compile_options(rms_test_audio_project_file PRIVATE -Wall -Wextra -pedantic)
endif()

add_test(
    NAME rms.audio.project_file
    COMMAND rms_test_audio_project_file
)

add_executable(rms_test_audio_project_manager
    test_audio_project_manager.cpp
)

target_link_libraries(rms_test_audio_project_manager
    PRIVATE
        rms_core
)

if(MSVC)
    target_compile_options(rms_test_audio_project_manager PRIVATE /W4)
else()
    target_compile_options(rms_test_audio_project_manager PRIVATE -Wall -Wextra -pedantic)
endif()

add_test(
    NAME rms.audio.project_manager
    COMMAND rms_test_audio_project_manager
)

add_executable(rms_test_audio_import_manager
    test_audio_import_manager.cpp
)

target_link_libraries(rms_test_audio_import_manager
    PRIVATE
        rms_core
)

if(MSVC)
    target_compile_options(rms_test_audio_import_manager PRIVATE /W4)
else()
    target_compile_options(rms_test_audio_import_manager PRIVATE -Wall -Wextra -pedantic)
endif()

add_test(
    NAME rms.audio.import_manager
    COMMAND rms_test_audio_import_manager
)

add_executable(rms_test_audio_import_config
    test_audio_import_config.cpp
)

target_link_libraries(rms_test_audio_import_config
    PRIVATE
        rms_core
)

if(MSVC)
    target_compile_options(rms_test_audio_import_config PRIVATE /W4)
else()
    target_compile_options(rms_test_audio_import_config PRIVATE -Wall -Wextra -pedantic)
endif()

add_test(
    NAME rms.audio.import_config
    COMMAND rms_test_audio_import_config
)

add_executable(rms_test_audio_import_format
    test_audio_import_format.cpp
)

target_link_libraries(rms_test_audio_import_format
    PRIVATE
        rms_core
)

if(MSVC)
    target_compile_options(rms_test_audio_import_format PRIVATE /W4)
else()
    target_compile_options(rms_test_audio_import_format PRIVATE -Wall -Wextra -pedantic)
endif()

add_test(
    NAME rms.audio.import_format
    COMMAND rms_test_audio_import_format
)
