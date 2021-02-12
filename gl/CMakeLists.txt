
add_library(pathfinder_gl STATIC 
	"Device.cpp"
	"Types.cpp"
	"Util.cpp"
)
target_link_libraries(pathfinder_gl PRIVATE pathfinder_core GLEW::GLEW)