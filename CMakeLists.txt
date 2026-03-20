cmake_minimum_required(VERSION 3.15...3.20)

project(glpp
    VERSION 0.9.2
    DESCRIPTION "Helper Classes that wrap OpenGL functionality"
    LANGUAGES CXX)

set(GLPP_IS_MASTER_PROJECT OFF)
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    set(GLPP_IS_MASTER_PROJECT ON)
endif()

option(GLPP_BUILD_DOCS "Builds the glpp documentation" ${GLPP_IS_MASTER_PROJECT})
option(GLPP_BUILD_EXAMPLES "Builds the glpp examples" ${GLPP_IS_MASTER_PROJECT})
option(GLPP_BUILD_TESTS "Builds the glpp tests" ${GLPP_IS_MASTER_PROJECT})

# Only if this is the top level project (not included with add_subdirectory)
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    # Use -std=c++xx instead of -std=g++xx
    set(CMAKE_CXX_EXTENSIONS OFF)

    # Support folders in IDEs
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    if(GLPP_BUILD_TESTS)
        # Testing only available for top level projects. It calls enable_testing
        # which must be in the main CMakeLists.
        include(CTest)
    endif()

    if(GLPP_BUILD_DOCS)
        # Generate documentation using Doxygen
        find_package(Doxygen)
        if(Doxygen_FOUND)
            add_subdirectory(docs)
        else()
            message(STATUS "Doxygen not found, not building docs")
        endif()
    endif()
endif()

# Use for platform independent install dirs, despite being called GNU
include(GNUInstallDirs)

# Create config.h with project version numbers
configure_file(cmake/config.h.in include/config.h)
include_directories(PRIVATE ${CMAKE_BINARY_DIR}/include)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)

add_subdirectory(external)

add_subdirectory(src)

if(GLPP_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Create Targets file
install(EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

# Generate ConfigVersion file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion)

# Copy the Config file to the build dir
configure_file(cmake/ProjectConfig.cmake ${PROJECT_NAME}Config.cmake @ONLY)

# Add Config and ConfigVersion files to install
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

# Add examples and tests
if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING) AND BUILD_TESTING)
    add_subdirectory(tests)
endif()
