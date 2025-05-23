cmake_minimum_required(VERSION 3.15)
project(RawMinecraft VERSION 0.1 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/UI
    ${CMAKE_SOURCE_DIR}/include/Logic
    ${CMAKE_SOURCE_DIR}/include/Form
    ${CMAKE_SOURCE_DIR}/include/Extended
    ${CMAKE_SOURCE_DIR}/SFML-2.6.2/include
    )
file(GLOB SOURCES 
    ${CMAKE_SOURCE_DIR}/source/*
    ${CMAKE_SOURCE_DIR}/source/UI/*
    ${CMAKE_SOURCE_DIR}/source/Logic/*
    ${CMAKE_SOURCE_DIR}/source/Extended/*
    ${CMAKE_SOURCE_DIR}/source/FormLogic/*
    ${CMAKE_SOURCE_DIR}/source/FormDesign/*
)
set(CMAKE_BUILD_TYPE Debug)
add_executable(${PROJECT_NAME} ${SOURCES})
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
)

target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/SFML-2.6.2/lib)
target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/SFML-2.6.2/bin)
target_link_libraries(${PROJECT_NAME} -lsfml-window -lsfml-graphics -lsfml-system -lOpenGl32 -lglu32 -lfreeglut)