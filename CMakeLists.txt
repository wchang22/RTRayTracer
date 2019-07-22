cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0015 NEW)

set(CMAKE_CXX_STANDARD 17)

project(rtraytracer)

include_directories(include)
link_directories(include)
include_directories(src)

find_package(assimp REQUIRED)
find_package(glfw3 REQUIRED)

file(GLOB_RECURSE SOURCES src/* shaders/*)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lglfw -lGL -ldl -lassimp -std=c++17")
set_source_files_properties(${SOURCES} PROPERTIES COMPILE_FLAGS
    "-Wall -Wextra -Werror -Wpedantic -Wno-ignored-qualifiers -Wno-deprecated-register")

file(GLOB_RECURSE SOURCES ${SOURCES} include/*)

option(RELEASE "Build in release mode" ON)
option(LOG "Enable logging" OFF)
option(PROFILE "Enable profiling" OFF)

if (RELEASE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
    set(LOG OFF)
    set(PROFILE OFF)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
    set(LOG ON)
    set(PROFILE ON)
endif()

if (LOG)
    add_definitions(-DLOG)
endif()

if (PROFILE)
    add_definitions(-DPROFILE)
endif()

message("Build Options -----------------------------------")
message("RELEASE ----------------------------------------- ${RELEASE}")
message("LOG --------------------------------------------- ${LOG}")
message("PROFILE ----------------------------------------- ${PROFILE}")

add_executable(${PROJECT_NAME} ${SOURCES})
