#pragma once
#include <cinttypes>
#include <glm/vec4.hpp>
#include "../geometry/LineSegment.hpp"
#include "../geometry/Transform2d.hpp"

namespace pf {
	enum class SegmentKind {
		None = 0,
		Line,
		Quadratic,
		Cubic,
	};
	enum class SegmentFlags: uint8_t {
		None = 0,
		FirstInSubpath = 1,
		ClosesSubpath = 2,
		All = 3
	};

	struct Segment {
		static Segment none() noexcept;
		static Segment line(const glm::vec2 & p0, const glm::vec2 & p1) noexcept;
		static Segment line(const LineSegment2F & p) noexcept;
		static Segment quadratic(const LineSegment2F& baseline, const glm::vec2& ctrl) noexcept;
		static Segment cubic(const LineSegment2F& baseline, const glm::vec2& c0, const glm::vec2 & c1) noexcept;

		Segment() noexcept;


		LineSegment2F baseline;
		LineSegment2F control;
		SegmentKind kind;
		SegmentFlags flags;
	};
};