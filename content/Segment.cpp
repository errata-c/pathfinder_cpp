#include "Segment.hpp"

#include <cmath>
#include <cassert>

#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/projection.hpp>

static constexpr float root2 = 1.414213562;
static constexpr float eps = 1E-4f;

/*
vec2 eval_horner(t) { 
	u = 1 - t; bc = 1; tn = 1; 
	tmp = controlpoints[0] * u; 
	for (int i = 1; i < n; i++) { 
		tn *= t; bc *= (n - i + 1) / i; 
		tmp = (tmp + tn * bc * controlpoints) * u; 
	} 
	return (tmp + tn * t * controlpoints[n]); 
}
*/
namespace pf {

	Segment Segment::none() noexcept {
		return {};
	}
	Segment Segment::line(const glm::vec2& p0, const glm::vec2& p1) noexcept {
		return Segment{p0, p1};
	}
	Segment Segment::line(const LineSegment2F& p) noexcept {
		return Segment{p.from(), p.to()};
	}
	Segment Segment::quadratic(const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2) noexcept {
		return Segment{p0, p1, p2};
	}
	Segment Segment::cubic(const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2, const glm::vec2 & p3) noexcept {
		return Segment{p0, p1, p2, p3};
	}
	Segment Segment::cubic(const LineSegment2F& b, const LineSegment2F& c) noexcept {
		return Segment{b.from(), c.from(), c.to(), b.to()};
	}

	Segment::Segment(const glm::vec2& p0, const glm::vec2& p1) noexcept
		: kind(SegmentKind::Line)
		, flags(SegmentFlags::None)
		, points{p0, p1}
	{}
	Segment::Segment(const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2) noexcept
		: kind(SegmentKind::Quadratic)
		, flags(SegmentFlags::None)
		, points{ p0, p1, p2 }
	{}
	Segment::Segment(const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3) noexcept
		: kind(SegmentKind::Cubic)
		, flags(SegmentFlags::None)
		, points{ p0, p1, p2, p3 }
	{}

	Segment Segment::quarterArc() noexcept {
		constexpr float os = 1.f / 6.f, ft = 4.f / 3.f;
		glm::vec2 p0{ 0.5f * root2 };
		glm::vec2 p1{-root2 * os + ft, 7.f * root2 * os - ft};
		return Segment::cubic(p0, glm::vec2{ p1.x, -p1.y }, p1, glm::vec2{p0.x, -p0.y});
	}
	Segment Segment::arc(float angle) noexcept {
		return Segment::arcFromCos(std::cos(angle));
	}
	Segment Segment::arcFromCos(float cosAngle) noexcept {
		// Richard A. DeVeneza, "How to determine the control points of a Bézier curve that
		// approximates a small arc", 2004.
		//
		// https://www.tinaja.com/glib/bezcirc2.pdf
		if (cosAngle >= 0.9999f) {
			return Segment::line({ glm::vec2{1, 0}, glm::vec2{1, 0} });
		}

		glm::vec4 term{cosAngle, -cosAngle, cosAngle, - cosAngle};
		glm::vec4 signs{1, -1, 1, -1};
		glm::vec4 p3p0 = glm::sqrt((1.f + term) * 0.5f) * signs;
		float p0x = p3p0.z, p0y = p3p0.w;
		float p1x = (4.f - p0x), p1y = (1.f - p0x) * (3.f - p0x) / p0y;
		glm::vec4 p2p1 = glm::vec4{p1x, -p1y, p1x, p1y} / 3.f;
		return Segment::cubic(p3p0,  p2p1);
	}

	Segment::Segment() noexcept
		: kind(SegmentKind::None)
		, flags(SegmentFlags::None)
		, points{ glm::vec2{0.f} }
	{}

	bool Segment::isNone() const noexcept {
		return kind == SegmentKind::None;
	}
	bool Segment::isLine() const noexcept {
		return kind == SegmentKind::Line;
	}
	bool Segment::isQuadratic() const noexcept {
		return kind == SegmentKind::Quadratic;
	}
	bool Segment::isCubic() const noexcept {
		return kind == SegmentKind::Cubic;
	}
	
	bool Segment::isTiny() const noexcept {
		float len2 = 0.f;
		for (auto it = begin() + 1, last = end(); it < last; ++it) {
			glm::vec2 dir = *(it - 1) - *it;
			len2 += glm::dot(dir, dir);
		}
		return len2 < 1e-3f;
	}

	// Returns this segment with control points reversed.
	Segment Segment::reversed() const noexcept {
		Segment seg = *this;
		auto write = seg.begin();
		for (auto it = rbegin(), last = rend(); it != last; ++it) {
			*write = *it;
		}
		return seg;
	}
	std::array<Segment, 2> Segment::split(float t) const noexcept {
		switch (kind) {
		case SegmentKind::Line: {
			glm::vec2 mp = glm::lerp(points[0], points[1], t);
			return {Segment::line(points[0], mp), Segment::line(mp, points[1])};
		}
		case SegmentKind::Quadratic: {
			glm::vec2 p0 = glm::lerp(points[0], points[1], t);
			glm::vec2 p1 = glm::lerp(points[1], points[2], t);

			glm::vec2 mp = glm::lerp(p0, p1, t);
			return { Segment{points[0], p0, mp}, Segment{mp, p1, points[2]} };
		}
		case SegmentKind::Cubic: {
			glm::vec2 p0 = glm::lerp(points[0], points[1], t);
			glm::vec2 p1 = glm::lerp(points[1], points[2], t);
			glm::vec2 p2 = glm::lerp(points[2], points[3], t);

			glm::vec2 p3 = glm::lerp(p0, p1, t);
			glm::vec2 p4 = glm::lerp(p1, p2, t);

			glm::vec2 mp = glm::lerp(p3, p4, t);

			return { Segment{points[0], p0, p3, mp}, Segment{mp, p2, p4, points[3]} };
		}
		default:
			return {};
		}
	}
	glm::vec2 Segment::sample(float t) const noexcept {
		switch (kind) {
		case SegmentKind::Line:
			return glm::lerp(points[0], points[1], t);
		case SegmentKind::Quadratic: {
			float t1 = 1.f - t;

			return 
				points[0] * t1 * t1 + 
				points[1] * 2.f * t * t1 + 
				points[2] * t * t;
		}
		case SegmentKind::Cubic: {
			float t1 = 1.f - t;
			float tt = t * t;
			float t1t1 = t1 * t1;

			return
				points[0] * t1t1 * t1 +
				points[1] * t1t1 * t * 3.f +
				points[2] * tt * t1 * 3.f +
				points[3] * tt * t;
		}
		default:
			return glm::vec2{ 0.f };
		}
	}
	Segment Segment::toCubic() const noexcept {
		switch (kind) {
		case SegmentKind::None:
			return Segment{};
		case SegmentKind::Line:
			return Segment{
				points[0], 
				glm::lerp(points[0], points[1], 1.f/ 3.f), 
				glm::lerp(points[0], points[1], 2.f / 3.f), 
				points[1]};
		case SegmentKind::Quadratic:
			return Segment{
				points[0],
				glm::lerp(points[0], points[1], 1.f / 3.f),
				glm::lerp(points[3], points[2], 1.f / 3.f),
				points[3]
			};
		case SegmentKind::Cubic:
			return *this;
		}
	}

	Segment Segment::splitBefore(float t) const noexcept {
		return split(t)[0];
	}
	Segment Segment::splitAfter(float t) const noexcept {
		return split(t)[1];
	}

	Segment Segment::transform(const Transform2F& form) const noexcept {
		Segment copy = *this;
		auto write = copy.begin();
		for (const glm::vec2 & point : *this) {
			*write = form.apply(point);
		}
		return copy;
	}

	using iterator = Segment::iterator;
	using const_iterator = Segment::const_iterator;
	using reverse_iterator = Segment::reverse_iterator;
	using const_reverse_iterator = Segment::const_reverse_iterator;

	std::size_t Segment::size() const noexcept {
		return static_cast<std::size_t>(kind);
	}
	bool Segment::empty() const noexcept {
		return size() == 0;
	}

	glm::vec2& Segment::front() {
		assert(!empty());
		return *begin();
	}
	glm::vec2& Segment::back() {
		assert(!empty());
		return *(end() - 1);
	}
	const glm::vec2& Segment::front() const {
		assert(!empty());
		return *begin();
	}
	const glm::vec2& Segment::back() const {
		assert(!empty());
		return *(end() - 1);
	}

	iterator Segment::begin() noexcept {
		return points.begin();
	}
	iterator Segment::end() noexcept {
		return points.begin() + size();
	}

	const_iterator Segment::begin() const noexcept {
		return points.cbegin();
	}
	const_iterator Segment::end() const noexcept {
		return points.cbegin() + size();
	}

	const_iterator Segment::cbegin() const noexcept {
		return points.cbegin();
	}
	const_iterator Segment::cend() const noexcept {
		return points.cbegin() + size();
	}

	reverse_iterator Segment::rbegin() noexcept {
		return points.rbegin();
	}
	reverse_iterator Segment::rend() noexcept {
		return points.rbegin() + size();
	}

	const_reverse_iterator Segment::rbegin() const noexcept {
		return points.rbegin();
	}
	const_reverse_iterator Segment::rend() const noexcept {
		return points.rbegin() + size();
	}

	const_reverse_iterator Segment::crbegin() const noexcept {
		return points.crbegin();
	}
	const_reverse_iterator Segment::crend() const noexcept {
		return points.crbegin() + size();
	}


	bool Segment::isFlat(float tolerance) const noexcept {
		switch (kind) {
		case SegmentKind::None:
			return false;
		case SegmentKind::Line:
			return true;
		case SegmentKind::Quadratic: {
			glm::vec2 u = points[1] - points[0];
			glm::vec2 v = points[2] - points[1];
			float det = u.x * v.y - u.y * v.x;

			return std::abs(det) < tolerance;
		}
		case SegmentKind::Cubic: {
			glm::vec2 u = points[1] - points[0];
			glm::vec2 v = points[2] - points[1];
			glm::vec2 w = points[3] - points[2];
			float det = std::abs(u.x * v.y - u.y * v.x);
			det += std::abs(u.x * w.y - u.y * w.x);
			return det < tolerance;
		}
		}
	}
	float Segment::minX() const noexcept {
		switch (kind) {
		case SegmentKind::None:
			return 0.f;
		case SegmentKind::Line:
			return std::min(points[0].x, points[1].x);
		case SegmentKind::Quadratic:
			return std::min(std::min(points[0].x, points[1].x), points[2].x);
		case SegmentKind::Cubic:
			return std::min(std::min(std::min(points[0].x, points[1].x), points[2].x), points[3].x);
		}
	}
	float Segment::minY() const noexcept {
		switch (kind) {
		case SegmentKind::None:
			return 0.f;
		case SegmentKind::Line:
			return std::min(points[0].y, points[1].y);
		case SegmentKind::Quadratic:
			return std::min(std::min(points[0].y, points[1].y), points[2].y);
		case SegmentKind::Cubic:
			return std::min(std::min(std::min(points[0].y, points[1].y), points[2].y), points[3].y);
		}
	}
	float Segment::maxX() const noexcept {
		switch (kind) {
		case SegmentKind::None:
			return 0.f;
		case SegmentKind::Line:
			return std::max(points[0].x, points[1].x);
		case SegmentKind::Quadratic:
			return std::max(std::max(points[0].x, points[1].x), points[2].x);
		case SegmentKind::Cubic:
			return std::max(std::max(std::max(points[0].x, points[1].x), points[2].x), points[3].x);
		}
	}
	float Segment::maxY() const noexcept {
		switch (kind) {
		case SegmentKind::None:
			return 0.f;
		case SegmentKind::Line:
			return std::max(points[0].y, points[1].y);
		case SegmentKind::Quadratic:
			return std::max(std::max(points[0].y, points[1].y), points[2].y);
		case SegmentKind::Cubic:
			return std::max(std::max(std::max(points[0].y, points[1].y), points[2].y), points[3].y);
		}
	}

	// ----------------------

};