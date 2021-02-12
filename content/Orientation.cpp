#include "Orientation.hpp"
#include "Outline.hpp"

namespace pf {
	Orientation orientationOf(const Outline& outline) {
		float area = 0.f;
		for (const Contour& contour : outline) {
			if (contour.empty()) {
				continue;
			}

			glm::vec2 u = contour.back().point;
			for (const Contour::Point& point : contour) {
				glm::vec2 v = point.point;
				area += u.x * v.y - u.y * v.x;

				u = v;
			}
		}
		return area <= 0.f ? Orientation::ccw : Orientation::cw;
	}
}