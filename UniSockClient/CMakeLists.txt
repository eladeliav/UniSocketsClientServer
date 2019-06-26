cmake_minimum_required(VERSION 3.0.0)
project(UniSockClient VERSION 0.1.0)

set(TARGET_NAME ${PROJECT_NAME})
set(${TARGET_NAME}_SRC
    client.cpp
)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-std=c++11")

if (UNIX)
    find_package(Threads REQUIRED)
endif ()

set(UniSockets_DIR $ENV{UNISOCKETS_DIR})
find_package(UniSockets REQUIRED)
if (UNIX)
    SET(CMAKE_CXX_FLAGS -pthread)
endif ()

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SRC})
target_link_libraries(${PROJECT_NAME} UniSockets::UniSockets ${CMAKE_THREAD_LIBS_INIT})