
add_library(pathfinder_geometry STATIC 
	"LineSegment.cpp"
	"Rect.cpp"
	"Transform2d.cpp"
	"Transform3d.cpp"
	"Util.cpp"
)
target_link_libraries(pathfinder_geometry PUBLIC pathfinder_core)