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
