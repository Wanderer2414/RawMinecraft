cmake_minimum_required(VERSION 3.15)
project(WorldRender VERSION 0.1 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

include_directories(
${CMAKE_SOURCE_DIR}/raylib/src/
)

file(GLOB SOURCES main.cpp
)

set(LIB 
raylib
)

if (UNIX)
endif()

if (WIN32)
endif()

#LIB HERE
add_subdirectory(${CMAKE_SOURCE_DIR}/raylib)

set(CMAKE_BUILD_TYPE Debug)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIB})
