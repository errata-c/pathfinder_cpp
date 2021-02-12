
add_library(pathfinder_color STATIC 
	"color.cpp" 
	"matrix.cpp"
)
target_link_libraries(pathfinder_color PUBLIC pathfinder_core)