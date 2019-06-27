cmake_minimum_required(VERSION 3.0.0)
project(UniSockServer VERSION 0.1.0)

# set source files
set(TARGET_NAME ${PROJECT_NAME})
set(${TARGET_NAME}_SRC
        server.cpp
        )

set(UniSockets_DIR $ENV{UNISOCKETS_DIR}) # set directory of UniSocketsLib
find_package(UniSockets REQUIRED) # find package

if (UNIX)
    SET(CMAKE_CXX_FLAGS -pthread) # this specific project requires pthread
endif ()

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SRC})
target_link_libraries(${PROJECT_NAME} UniSockets::UniSockets)