enable_testing()
include(GoogleTest)

file(GLOB_RECURSE SOURCE_LIST ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HEADER_LIST ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

foreach(TEST_SOURCE ${SOURCE_LIST})
    cmake_path(GET TEST_SOURCE STEM test_name)
    if(${test_name} MATCHES "^test_")
        message("Generate test ${test_name}")

        add_executable(${test_name} ${TEST_SOURCE} ${HEADER_LIST}
            glTest.cpp
            glTest.hpp
            glmChecker.cpp
            glmChecker.hpp)
        target_link_libraries(${test_name} PRIVATE gtest gtest_main glpp glfw)

        gtest_discover_tests(${test_name})
    endif()
endforeach()

