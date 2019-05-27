cmake_minimum_required(VERSION 3.0.0)
project(UniSockServer VERSION 0.1.0)

set(TARGET_NAME ${PROJECT_NAME})
set(${TARGET_NAME}_SRC
        server.cpp
        )

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-std=c++11")

if (WIN32)
    set(UniSockets_DIR "D:/Programming/VisualStudioProjects/CPP/UniSockets/build/")
else ()
    set(UniSockets_DIR "/Users/eladeliav/Documents/Programming/C++/UniSockets/build")
endif ()
find_package(UniSockets REQUIRED)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SRC})
target_link_libraries(${PROJECT_NAME} UniSockets::UniSockets)