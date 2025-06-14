cmake_minimum_required(VERSION 3.15)
project(RawMinecraft VERSION 0.1 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/UI
    ${CMAKE_SOURCE_DIR}/include/Logic
    ${CMAKE_SOURCE_DIR}/include/Form
    ${CMAKE_SOURCE_DIR}/include/Extended
    ${CMAKE_SOURCE_DIR}/glad/include
    ${CMAKE_SOURCE_DIR}/glm-master
    ${CMAKE_SOURCE_DIR}/glfw/include
)
file(GLOB SOURCES 
    ${CMAKE_SOURCE_DIR}/source/*
    ${CMAKE_SOURCE_DIR}/source/UI/*
    ${CMAKE_SOURCE_DIR}/source/Logic/*
    ${CMAKE_SOURCE_DIR}/source/Extended/*
    ${CMAKE_SOURCE_DIR}/source/FormLogic/*
    ${CMAKE_SOURCE_DIR}/source/FormDesign/*
    ${CMAKE_SOURCE_DIR}/glad/src/*.c
)
set(LIB glfw)
if (UNIX) 
    set(LIB X11 glfw)
endif()
set(CMAKE_BUILD_TYPE Debug)
add_subdirectory(${CMAKE_SOURCE_DIR}/glm-master)
add_subdirectory(${CMAKE_SOURCE_DIR}/glfw)
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include/Extended/)
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIB})