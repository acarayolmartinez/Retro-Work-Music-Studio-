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
