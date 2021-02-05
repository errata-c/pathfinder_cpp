cmake_minimum_required(VERSION 3.15)

project(PATHFINDER_CPP
VERSION 0.1
LANGUAGES C CXX)

include(GNUInstallDirs)
find_package(ez-cmake CONFIG REQUIRED)

find_package(fmt CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_library(pathfinder-color STATIC "color/color.cpp" "color/matrix.cpp")
target_link_libraries(pathfinder-color PUBLIC glm fmt::fmt)
target_compile_features(pathfinder-color PUBLIC cxx_std_17)
target_include_directories(pathfinder-color PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

add_library(pathfinder-geometry STATIC 
	"geometry/LineSegment.cpp"
	"geometry/Rect.cpp"
	"geometry/Transform2d.cpp"
	"geometry/Transform3d.cpp"
	"geometry/Util.cpp"
)
target_link_libraries(pathfinder-geometry PUBLIC glm fmt::fmt)
target_compile_features(pathfinder-geometry PUBLIC cxx_std_17)
target_include_directories(pathfinder-geometry PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

add_library(pathfinder-content STATIC 
	"content/Clip.cpp"
	"content/Dash.cpp"
	"content/Dilation.cpp"
	"content/Effects.cpp"
	"content/Fill.cpp"
	"content/Gradient.cpp"
	"content/Orientation.cpp"
	"content/Outline.cpp"
	"content/Pattern.cpp"
	"content/Segment.cpp"
	"content/Stroke.cpp"
	"content/Transform.cpp"
)
target_link_directories(pathfinder-content PUBLIC glm fmt::fmt pathfinder-color pathfinder-geometry)
target_include_directories(pathfinder-content PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")



