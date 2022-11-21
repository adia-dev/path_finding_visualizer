# Example usage:
# mkdir build
# cd build
# cmake -g "Visual Studio 14 2015" ..

cmake_minimum_required(VERSION 3.0)

set(PROJECT_NAME app)

project(${PROJECT_NAME} C CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DVK_PROTOTYPES")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_PROTOTYPES")

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

file(GLOB sources src/*.cpp)

set(IMGUI_SRC ${IMGUI_DIR}/imgui.cpp ${IMGUI_DIR}/imgui_demo.cpp ${IMGUI_DIR}/imgui_draw.cpp ${IMGUI_DIR}/imgui_tables.cpp ${IMGUI_DIR}/imgui_widgets.cpp)
set(GLFW_SRC ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp)

add_definitions(-D__DIR__=\"$(SOURCES_DIR)/\" -D__LOG__=\"\" -D__LOG_TO_FILE__)

add_executable(${PROJECT_NAME} ${sources} ${IMGUI_SRC} ${GLFW_SRC})
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})

target_compile_definitions(${PROJECT_NAME} PUBLIC -DImTextureID=ImU64)
