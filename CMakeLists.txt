cmake_minimum_required(VERSION 3.15)
project(MyCraft VERSION 0.1 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

file(GLOB_RECURSE SOURCES "source/*.cpp" "glad/*.c" )
file(GLOB_RECURSE HEADER_FILE "include/*.h")

# Filter directories only
set(HEADERS "")
foreach(PATH ${HEADER_FILE})
    get_filename_component(FOLDER ${PATH} DIRECTORY)
    list(APPEND HEADERS ${FOLDER})
endforeach()

include_directories(
    ${HEADERS}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/glad/include
    ${CMAKE_SOURCE_DIR}/glm-master
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