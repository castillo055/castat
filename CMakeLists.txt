cmake_minimum_required(VERSION 3.13)
project(castat)
set(CMAKE_CXX_STANDARD 20)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(SOURCE_FILES src/castat.cpp)

add_executable(castat ${SOURCE_FILES})
set_target_properties(castat PROPERTIES
        CXX_EXTENSIONS OFF)

target_link_libraries(castat -lcipcomms)
target_link_libraries(castat -lrt)
target_link_libraries(castat -L/home/victor/repos/inhouse/cipcomms)
TARGET_LINK_LIBRARIES(castat Threads::Threads asound X11)

FIND_PACKAGE( Boost 1.40 COMPONENTS program_options REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

TARGET_LINK_LIBRARIES( castat  ${Boost_LIBRARIES} )

