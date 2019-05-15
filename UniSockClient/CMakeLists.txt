cmake_minimum_required(VERSION 3.0.0)
project(UniSockClient VERSION 0.1.0)

set(TARGET_NAME ${PROJECT_NAME})
set(${TARGET_NAME}_SRC
    client.cpp
)

set(LibUniSockets_DIR "D:/Programming/VisualStudioProjects/CPP/VSCodeVersions/UniSockets/build")
find_package(LibUniSockets REQUIRED)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SRC})
target_link_libraries(${PROJECT_NAME} LibUniSockets::LibUniSockets)