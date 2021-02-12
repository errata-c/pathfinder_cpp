

add_library(pathfinder_content STATIC 
	"Clip.cpp"
	"Dash.cpp"
	"Dilation.cpp"
	#"Effects.cpp"
	"Fill.cpp"
	"Gradient.cpp"
	"Orientation.cpp"
	"Outline.cpp"
	"Contour.cpp"
	"Pattern.cpp"
	"Segment.cpp"
	"Stroke.cpp"
	"Transform.cpp"
)
target_link_libraries(pathfinder_content PUBLIC pathfinder_core pathfinder_geometry pathfinder_color)