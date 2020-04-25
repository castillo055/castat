cmake_minimum_required(VERSION 3.13)
project(castat)

set(SOURCE_FILES castat.cpp)

add_executable(castat ${SOURCE_FILES})
set_target_properties(castat PROPERTIES
        CXX_STANDARD 17
        CXX_EXTENSIONS OFF)