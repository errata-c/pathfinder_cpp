add_library(pathfinder_gpu STATIC 
	"half.cpp"
	"Enums.cpp"
	"GPU.cpp"
)
target_link_libraries(pathfinder_gpu PUBLIC pathfinder_core pathfinder_geometry)

add_subdirectory("test")