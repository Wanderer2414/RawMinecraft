cmake_minimum_required(VERSION 3.16)

project(RawMinecraft)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(
    ${CMAKE_SOURCE_DIR}/include/Logic
    ${CMAKE_SOURCE_DIR}/include/UI
)

file(GLOB FILES 
    ${CMAKE_SOURCE_DIR}/source/Logic/*
    ${CMAKE_SOURCE_DIR}/source/UI/*
)

add_executable(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/source/main.cpp)
target_link_libraries(${PROJECT_NAME} ${FILES} -lGL -lglut -LGLU)