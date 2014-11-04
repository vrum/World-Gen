cmake_minimum_required(VERSION 2.8.11)

# A macro that sets the default value of a variable
macro(variable_default_value var default)
    if(NOT DEFINED ${var})
        set(${var} ${default})
    endif()
endmacro()

# Build type
variable_default_value(CMAKE_BUILD_TYPE Release)

# Enable project folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")

# Set up the module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake/Modules")

# Set the project
project(World-Gen)

# Set the version number
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

# Qt Stuff
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Set the source files
set(SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Array.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MapWriter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Noise.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Vector.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/World.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/UI/QSFML.cpp
)

# Set the output directory
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")

# Set up compiler flags
# @todo Settings for MSVC, etc?
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Weverything -pedantic -Wno-c++98-compat -Wno-padded -Wno-weak-vtables")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")
endif()

# Locate libraries
find_package(SFML 2 COMPONENTS graphics window system REQUIRED)
include_directories(${SFML_INCLUDE_DIR})
set(LIBS ${LIBS} ${SFML_LIBRARIES})
find_package(Qt5Widgets REQUIRED)
set(LIBS ${LIBS} Qt5::Widgets)

# Build the game engine
add_executable(World-Gen ${SRC})

# Add linker targets
target_link_libraries(World-Gen ${LIBS})
