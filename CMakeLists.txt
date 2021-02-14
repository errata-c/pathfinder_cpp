cmake_minimum_required(VERSION 3.17)

project(PATHFINDER_CPP
VERSION 0.1
LANGUAGES C CXX)

include(GNUInstallDirs)
find_package(ez-cmake CONFIG REQUIRED)

find_package(fmt CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glew CONFIG REQUIRED)

add_library(pathfinder_core INTERFACE)
target_compile_features(pathfinder_core INTERFACE cxx_std_17)
target_link_libraries(pathfinder_core INTERFACE glm fmt::fmt)
target_include_directories(pathfinder_core INTERFACE 
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
	"$<INSTALL_INTERFACE:include>"
)
target_compile_definitions(pathfinder_core INTERFACE
	"$<$<CONFIG:Debug>:PF_DEBUG_ASSERTIONS>"
)

#adds pathfinder_color target
add_subdirectory("color")

#adds pathfinder_geometry target
add_subdirectory("geometry")

#adds pathfinder_content target
add_subdirectory("content")

#adds pathfinder_gpu target
add_subdirectory("gpu")

#adds pathfinder_gl target
add_subdirectory("gl")
