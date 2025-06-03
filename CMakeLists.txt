cmake_minimum_required(VERSION 3.15)
project(RawMinecraft VERSION 0.1 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
if (WIN32 OR MSYS)
    find_package(GLFW3 REQUIRED)
endif()
if (UNIX AND NOT MSYS)
    if (NOT EXISTS ${CMAKE_SOURCE_DIR}/glfw/build)
        file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/glfw/build)
    endif()
    if (NOT EXISTS ${CMAKE_SOURCE_DIR}/glfw/build/Makefile) 
        execute_process(COMMAND cmake -S ${CMAKE_SOURCE_DIR}/glfw -B ${CMAKE_SOURCE_DIR}/glfw/build -G "Unix Makefiles")
    endif()
    execute_process(COMMAND make -j${nproc} -C ${CMAKE_SOURCE_DIR}/glfw/build )
endif()
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
set(CMAKE_BUILD_TYPE Debug)
add_subdirectory(${CMAKE_SOURCE_DIR}/glm-master)
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include/Extended/)
target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/glfw/build/src)
target_link_libraries(${PROJECT_NAME} -lglfw3)
