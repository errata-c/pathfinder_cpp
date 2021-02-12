#pragma once
#include <cinttypes>
#include <array>

#include "../geometry/LineSegment.hpp"
#include "../geometry/Transform2d.hpp"

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>


namespace pf {
	enum class SegmentKind {
		None = 0,
		Line = 2,
		Quadratic = 3,
		Cubic = 4,
	};
	enum class SegmentFlags: uint8_t {
		None = 0,
		FirstInSubpath = 1,
		ClosesSubpath = 2,
		All = 3
	};

	struct Segment {
		using container_t = std::array<glm::vec2, 4>;
		using iterator = container_t::iterator;
		using const_iterator = container_t::const_iterator;
		using reverse_iterator = container_t::reverse_iterator;
		using const_reverse_iterator = container_t::const_reverse_iterator;

		static Segment none() noexcept;
		static Segment line(const glm::vec2 & p0, const glm::vec2 & p1) noexcept;
		static Segment line(const LineSegment2F & p) noexcept;
		static Segment quadratic(const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2) noexcept;
		static Segment cubic(const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2, const glm::vec2 & p3) noexcept;
		static Segment cubic(const LineSegment2F& b, const LineSegment2F& c) noexcept;

		static Segment quarterArc() noexcept;
		static Segment arc(float angle) noexcept;
		static Segment arcFromCos(float cosAngle) noexcept;

		Segment() noexcept;
		Segment(const glm::vec2 & p0, const glm::vec2 & p1) noexcept;
		Segment(const glm::vec2& p0, const glm::vec2& p1, const glm::vec2 & p2) noexcept;
		Segment(const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2, const glm::vec2 & p3) noexcept;

		bool isNone() const noexcept;
		bool isLine() const noexcept;
		bool isQuadratic() const noexcept;
		bool isCubic() const noexcept;

		// Returns true if segment if less than epsilon (defined in Segment.cpp)
		bool isTiny() const noexcept;

		// Returns this segment with control points reversed.
		Segment reversed() const noexcept;
		std::array<Segment, 2> split(float t) const noexcept;
		Segment splitBefore(float t) const noexcept;
		Segment splitAfter(float t) const noexcept;
		glm::vec2 sample(float t) const noexcept;
		Segment toCubic() const noexcept;

		Segment transform(const Transform2F& form) const noexcept;

		bool isFlat(float tolerance) const noexcept;
		float minX() const noexcept;
		float minY() const noexcept;
		float maxX() const noexcept;
		float maxY() const noexcept;

		std::size_t size() const noexcept;
		bool empty() const noexcept;

		glm::vec2& front();
		glm::vec2& back();
		const glm::vec2& front() const;
		const glm::vec2& back() const;

		iterator begin() noexcept;
		iterator end() noexcept;

		const_iterator begin() const noexcept;
		const_iterator end() const noexcept;

		const_iterator cbegin() const noexcept;
		const_iterator cend() const noexcept;

		reverse_iterator rbegin() noexcept;
		reverse_iterator rend() noexcept;

		const_reverse_iterator rbegin() const noexcept;
		const_reverse_iterator rend() const noexcept;

		const_reverse_iterator crbegin() const noexcept;
		const_reverse_iterator crend() const noexcept;

		std::array<glm::vec2, 4> points;
		SegmentKind kind;
		SegmentFlags flags;
	};
};