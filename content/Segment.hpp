#pragma once
#include <cinttypes>
#include <glm/vec4.hpp>
#include <array>
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

	struct QuadraticSegment;
	struct CubicSegment;

	struct Segment {
		static Segment none() noexcept;
		static Segment line(const glm::vec2 & p0, const glm::vec2 & p1) noexcept;
		static Segment line(const LineSegment2F & p) noexcept;
		static Segment quadratic(const LineSegment2F& baseline, const glm::vec2& ctrl) noexcept;
		static Segment cubic(const LineSegment2F& baseline, const glm::vec2& c0, const glm::vec2 & c1) noexcept;

		static Segment quarterArc() noexcept;
		static Segment arc(float angle) noexcept;
		static Segment arcFromCos(float cosAngle) noexcept;

		Segment() noexcept;

		bool isNone() const noexcept;
		bool isLine() const noexcept;
		bool isQuadratic() const noexcept;
		bool isCubic() const noexcept;

		// Should these be noexcept? If they throw an exception its a logic error, not a runtime error.
		const LineSegment2F& asLine() const;
		const QuadraticSegment& asQuadratic() const;
		const CubicSegment& asCubic() const;

		// Returns true if segment if less than epsilon (defined in Segment.cpp)
		bool isTiny() const noexcept;

		// Returns this segment with control points reversed.
		Segment reversed() const noexcept;
		std::array<Segment, 2> split(float t) const noexcept;
		glm::vec2 sample(float t) const noexcept;
		Segment toCubic() const noexcept;

		LineSegment2F baseline;
		LineSegment2F control;
		SegmentKind kind;
		SegmentFlags flags;
	};

	// Additional functions specific to quadratic bezier
	struct QuadraticSegment: public Segment {
		bool isFlat() const noexcept;
		std::array<Segment, 2> split(float t) const noexcept;
		Segment splitBefore(float t) const noexcept;
		Segment splitAfter(float t) const noexcept;
		glm::vec2 sample(float t) const noexcept;

		float minX() const noexcept;
		float minY() const noexcept;
		float maxX() const noexcept;
		float maxY() const noexcept;
	};
	
	// Additional functions specific to cubic bezier
	struct CubicSegment: public Segment {
		bool isFlat() const noexcept;
		std::array<Segment, 2> split(float t) const noexcept;
		Segment splitBefore(float t) const noexcept;
		Segment splitAfter(float t) const noexcept;
		glm::vec2 sample(float t) const noexcept;

		float minX() const noexcept;
		float minY() const noexcept;
		float maxX() const noexcept;
		float maxY() const noexcept;
	};
};