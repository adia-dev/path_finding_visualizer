# Example usage:
# mkdir build
# cd build
# cmake -g "Visual Studio 14 2015" ..

cmake_minimum_required(VERSION 3.0)

set(PROJECT_NAME app)

project(${PROJECT_NAME} C CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CONFIGURATION_TYPES "Debug;Release;Playground")
    set(CMAKE_CONFIGURATION_TYPES ${CONFIGURATION_TYPES} CACHE STRING "Configurations" FORCE)
    set(CMAKE_BUILD_TYPE "Playground" CACHE STRING "Choose the type of build." FORCE)
else()
    set(CONFIGURATION_TYPES ${CMAKE_CONFIGURATION_TYPES})
endif()

set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${CONFIGURATION_TYPES})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DVK_PROTOTYPES")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_PROTOTYPES")

# CMAKE_EXPORT_COMPILE_COMMANDS
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Directories
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# GLFW
set(GLFW_DIR include/glfw) # Set this to point to an up-to-date GLFW repo
add_subdirectory(${GLFW_DIR} binary_dir EXCLUDE_FROM_ALL)
include_directories(${GLFW_DIR})

# Dear ImGui
set(IMGUI_DIR include/imgui)
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends ..)

# Libraries
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

# find_library(VULKAN_LIBRARY
# NAMES vulkan vulkan-1)
# set(LIBRARIES "glfw;${VULKAN_LIBRARY}")
set(LIBRARIES "glfw;OpenGL::GL")

# echo LIBRARIES
message(STATUS "LIBRARIES: ${LIBRARIES}")

file(GLOB_RECURSE SRC_FILES ${SRC_DIR}/*.cpp ${SRC_DIR}/*.h)

set(IMGUI_SRC ${IMGUI_DIR}/imgui.cpp ${IMGUI_DIR}/imgui_demo.cpp ${IMGUI_DIR}/imgui_draw.cpp ${IMGUI_DIR}/imgui_tables.cpp ${IMGUI_DIR}/imgui_widgets.cpp)
set(GLFW_SRC ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp)

add_definitions(-D__DIR__=\"\" -D__LOG__=\"\" -D__LOG_TO_FILE__)

add_executable(${PROJECT_NAME} ${SRC_FILES} ${IMGUI_SRC} ${GLFW_SRC})
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})

if(${CMAKE_BUILD_TYPE} STREQUAL "Playground")
    target_compile_options(${PROJECT_NAME} PRIVATE -D__PLAYGROUND_MODE__)
endif()

target_compile_definitions(${PROJECT_NAME} PUBLIC -DImTextureID=ImU64)
