cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0015 NEW)

project(rtraytracer)

find_package(glfw3 REQUIRED)

file(GLOB_RECURSE SOURCES src/* shaders/*)

set_source_files_properties(${SOURCES} PROPERTIES COMPILE_FLAGS
    "-Wall -Wextra -Werror -Wpedantic -Wno-ignored-qualifiers -Wno-deprecated-register")

file(GLOB_RECURSE SOURCES ${SOURCES} include/*)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_options(${PROJECT_NAME} PRIVATE -lglfw -lGL -ldl -lassimp)
target_compile_options(${PROJECT_NAME} PRIVATE -std=c++17)
target_include_directories(${PROJECT_NAME} PRIVATE src include)
target_link_directories(${PROJECT_NAME} PRIVATE include)

option(RELEASE "Build in release mode" ON)
option(LOG "Enable logging" OFF)
option(PROFILE "Enable profiling" OFF)

if (RELEASE)
    target_compile_options(${PROJECT_NAME} PRIVATE -O3)
    set(LOG OFF)
    set(PROFILE OFF)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -g)
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
