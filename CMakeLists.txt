cmake_minimum_required(VERSION 3.15)
project(MyCraft VERSION 0.1 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_BUILD_TYPE Debug)

# ----------------------------
# Source + Header
# ----------------------------
file(GLOB_RECURSE SOURCES "source/*.cpp" "glad/*.c")
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
    ${CMAKE_SOURCE_DIR}/bass24/c
)

set(LIB glfw)
if (UNIX) 
    set(LIB X11 glfw)
endif()

# ----------------------------
# External libs
# ----------------------------
add_subdirectory(${CMAKE_SOURCE_DIR}/glm-master)
add_subdirectory(${CMAKE_SOURCE_DIR}/glfw)

# ----------------------------
# Main exe
# ----------------------------
add_executable(${PROJECT_NAME} ${SOURCES})

# ----------------------------
# BASS24
# ----------------------------
if(WIN32)
    if(MINGW)
        # Nếu đã có libbass.a thì dùng luôn
        if(EXISTS "${CMAKE_SOURCE_DIR}/bass24/libbass.a")
            message(STATUS "Using pre-built libbass.a")
            target_link_libraries(${PROJECT_NAME} PRIVATE
                ${CMAKE_SOURCE_DIR}/bass24/libbass.a
                winmm
            )
        else()
            # Nếu chưa có thì tự tạo từ bass.dll
            add_custom_command(
                OUTPUT ${CMAKE_BINARY_DIR}/libbass.a
                COMMAND gendef ${CMAKE_SOURCE_DIR}/bass24/bass.dll
                COMMAND dlltool -d ${CMAKE_SOURCE_DIR}/bass24/bass.def -l ${CMAKE_BINARY_DIR}/libbass.a -D ${CMAKE_SOURCE_DIR}/bass24/bass.dll
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bass24
                COMMENT "Building libbass.a from bass.dll"
            )

            add_custom_target(basslib DEPENDS ${CMAKE_BINARY_DIR}/libbass.a)
            add_dependencies(${PROJECT_NAME} basslib)

            target_link_libraries(${PROJECT_NAME} PRIVATE
                ${CMAKE_BINARY_DIR}/libbass.a
                winmm
            )
        endif()
    else()
        # Cho MSVC (dùng bass.lib đi kèm)
        target_link_libraries(${PROJECT_NAME} PRIVATE
            ${CMAKE_SOURCE_DIR}/bass24/bass.lib
            winmm
        )
    endif()

    # Copy DLL ra thư mục runtime
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/bass24/bass.dll
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    )
endif()

# ----------------------------
# Link các lib khác (GLFW, GLM,…)
# ----------------------------
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIB})
