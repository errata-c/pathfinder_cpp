#include "Contour.hpp"
#include "Util.hpp"

#include <cassert>
#include <limits>

#include <glm/gtx/matrix_operation.hpp>

namespace pf {
	Contour Contour::withCapacity(std::size_t cap) {
		Contour ret;
		ret.points.reserve(cap);
		return ret;
	}
	Contour Contour::fromRect(const RectF& rect) {
		Contour ret = withCapacity(4);
		ret.pushPoint(rect.upperLeft(), 0, false);
		ret.pushPoint(rect.upperRight(), 0, false);
		ret.pushPoint(rect.lowerRight(), 0, false);
		ret.pushPoint(rect.lowerLeft(), 0, false);
		ret.close();
		ret.bounds = rect;
		return ret;
	}
	Contour Contour::fromRectRounded(const RectF& rect, glm::vec2 radius) {
		constexpr float quarterArcCPOutside = (3.f - 4.f * (root2 - 1.f)) / 3.f;

		if (std::abs(radius.x) < 1e-4f && std::abs(radius.y) < 1e-4f) {
			return Contour::fromRect(rect);
		}

		radius = glm::min(radius, rect.size() * 0.5f);
		glm::vec2 cpOffset = radius * quarterArcCPOutside;

		Contour contour = Contour::withCapacity(8);

		// Upper left corner
		{
			glm::vec2 p0 = rect.upperLeft();
			glm::vec2 p1 = p0 + cpOffset;
			glm::vec2 p2 = p0 + radius;
			contour.pushEndpoint({p0.x, p2.y});
			contour.pushCubic(
				{p0.x, p1.y}, 
				{p1.x, p0.y}, 
				{p2.x, p0.y});
		}

		// upper right
		{
			glm::vec2 p0 = rect.upperRight();
			glm::vec2 p1 = p0 + glm::vec2{-cpOffset.x, cpOffset.y};
			glm::vec2 p2 = p0 + glm::vec2{-radius.x, radius.y};
			contour.pushEndpoint({ p2.x, p0.y });
			contour.pushCubic(
				{ p1.x, p0.y },
				{ p0.x, p1.y },
				{ p0.x, p2.y });
		}

		// lower right
		{
			glm::vec2 p0 = rect.lowerRight();
			glm::vec2 p1 = p0 + glm::vec2{ -cpOffset.x, -cpOffset.y };
			glm::vec2 p2 = p0 + glm::vec2{ -radius.x, -radius.y };
			contour.pushEndpoint({ p0.x, p2.y });
			contour.pushCubic(
				{ p0.x, p1.y },
				{ p1.x, p0.y },
				{ p2.x, p0.y });
		}

		// lower left
		{
			glm::vec2 p0 = rect.lowerLeft();
			glm::vec2 p1 = p0 + glm::vec2{ cpOffset.x, -cpOffset.y };
			glm::vec2 p2 = p0 + glm::vec2{ radius.x, -radius.y };
			contour.pushEndpoint({ p2.x, p0.y });
			contour.pushCubic(
				{ p1.x, p0.y },
				{ p0.x, p1.y },
				{ p0.x, p2.y });
		}

		contour.close();
		return contour;
	}

	Contour::Contour()
		: bounds{}
		, closed(false)
	{}

	void Contour::clear() noexcept {
		closed = false;
		bounds = RectF{};
		points.clear();
	}
	bool Contour::empty() const noexcept {
		return points.empty();
	}
	bool Contour::isClosed() const noexcept {
		return closed;
	}
	std::size_t Contour::size() const noexcept {
		return points.size();
	}

	void Contour::close() {
		closed = true;
	}

	Contour::Point& Contour::front() {
		assert(size() > 0);
		return points.front();
	}
	const Contour::Point& Contour::front() const {
		assert(size() > 0);
		return points.front();
	}

	Contour::Point& Contour::back() {
		assert(size() > 0);
		return points.back();
	}
	const Contour::Point& Contour::back() const {
		assert(size() > 0);
		return points.back();
	}

	Contour::Point& Contour::operator[](std::size_t index) {
		assert(index < size());
		return points[index];
	}
	const Contour::Point& Contour::operator[](std::size_t index) const {
		assert(index < size());
		return points[index];
	}

	void Contour::pushPoint(const glm::vec2& p, int kind, bool updateBounds) {
		if (updateBounds) {
			if (empty()) {
				bounds = RectF::fromPoints(p, p);
			}
			else {
				bounds = bounds.merge(p);
			}
		}
		points.push_back(Point{ p, kind });
	}
	void Contour::pushSegment(const Segment& seg, bool updateBounds) {
		if (seg.isNone()) {
			return;
		}

		int kind = 0;
		auto it = seg.begin();
		for (auto last = seg.end() - 1; it != last; ++it) {
			pushPoint(*it, kind, updateBounds);
			++kind;
		}
		pushPoint(*it, 0, updateBounds);
	}
	void Contour::pushEllipse(const Transform2F& form) {
		Segment segment = Segment::quarterArc();
		Transform2F rotation;
		pushSegment(segment.transform(form), true);

		rotation = Transform2F::fromRotation(glm::vec2{ 0.f, 1.f });
		pushSegment(segment.transform(form.apply(rotation)), true);

		rotation = Transform2F::fromRotation(glm::vec2{ -1.f, 0.f });
		pushSegment(segment.transform(form.apply(rotation)), true);

		rotation = Transform2F::fromRotation(glm::vec2{ 0.f, -1.f });
		pushSegment(segment.transform(form.apply(rotation)), true);
	}
	void Contour::pushArc(const Transform2F& form, float s, float e, ArcDirection dir) {
		if ((e - s) >= tau) {
			pushEllipse(form);
		}
		else {
			glm::vec2 first{ std::cos(s), std::sin(s) }, last{ std::cos(e), std::sin(e) };
			pushArcFromUnitChord(form, LineSegment2F{ first, last }, dir);
		}
	}
	void Contour::pushArcFromUnitChord(const Transform2F& form, LineSegment2F chord, ArcDirection dir) {
		Transform2F dirForm;
		if (dir == ArcDirection::CCW) {
			chord *= glm::vec2{1.f, -1.f};
			dirForm = Transform2F::fromScale(glm::vec2{1.f, -1.f});
		}

		glm::vec2
			norm = glm::normalize(chord.from()),
			endNorm = glm::normalize(chord.to());

		for (int i = 0; i < 4; ++i) {
			glm::vec2 sweep = revRotateBy(endNorm, norm);
			bool last = std::abs(sweep.x) < 1e-3f;

			Segment segment;
			if (!last) {
				sweep = glm::vec2{0.f, 1.f};
				segment = Segment::quarterArc();
			}
			else {
				segment = Segment::arcFromCos(sweep.x);
			}

			glm::vec2 halfSweep = halveAngle(sweep);
			Transform2F rotation = Transform2F::fromRotation(rotateBy(halfSweep, norm));

			segment = segment.transform(form.apply(dirForm).apply(rotation));

			pushSegment(segment, true);
			if (last) {
				break;
			}

			norm = rotateBy(norm, sweep);
		}
	}

	void Contour::pushSVGArc(const glm::vec2& radius, float x_rot, bool large_arc, ArcDirection dir, const glm::vec2& to) {
		glm::vec2 r = radius;
		glm::vec2 p = to;
		glm::vec2 last{0.f};
		if (!empty()) {
			last = back().point;
		}

		if (r.x < std::numeric_limits<float>::infinity() && r.y < std::numeric_limits<float>::infinity()) {
			r = glm::abs(r);
			glm::vec2 rinv = 1.f / r;

			float sign = large_arc ? 1.f: -1.f;
			sign = dir == ArcDirection::CW ? -sign : sign;

			glm::mat2 rot;
			rot[0] = glm::vec2{std::cos(x_rot), std::sin(x_rot)};
			rot[1] = glm::vec2{-rot[0].y, rot[0].x};

			glm::vec2 q = glm::adjugate(rot) * (last - p) * 0.5f;
			glm::vec2 q2 = q * q;

			float gamma = 0.f;
			{
				glm::vec2 gammav = q2 * rinv * rinv;
				gamma = gammav.x + gammav.y;
			}
			
			glm::vec2 a, b, c;
			if (gamma <= 1.f) {
				glm::vec2 r2 = r * r;
				float r2_prod = r2.x + r2.y;
				glm::vec2 rq2 = r2 * glm::vec2{q2.y, q2.x};
				float rq2_sum = rq2.x + rq2.y;

				// c
				glm::vec2 s = glm::vec2{ 1, -1 } *r * glm::vec2{ q.y * rinv.y, q.x * rinv.x } *safeSqrt((r2_prod - rq2_sum) / rq2_sum) * sign;
				c = rot * s + (last + p) * 0.5f;

				a = (q - s) * rinv;
				b = -(q - s) * rinv;
			}
			else {
				c = (last + p) * 0.5f;
				a = q * rinv;
				b = -a;
			}

			Transform2F form{ rot, c };
			form = form.apply(Transform2F::fromScale(r));
			LineSegment2F chord{ a, b };
			pushArcFromUnitChord(form, chord, dir);
		}
		else {
			pushEndpoint(p);
		}
	}

	void Contour::pushQuadratic(const glm::vec2& p0, const glm::vec2& p1) {
		pushPoint(p0, 1, true);
		pushPoint(p1, 0, true);
	}
	void Contour::pushCubic(const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2) {
		pushPoint(p0, 1, true);
		pushPoint(p1, 2, true);
		pushPoint(p2, 0, true);
	}
	void Contour::pushEndpoint(const glm::vec2& p) {
		pushPoint(p, 0, true);
	}

	void Contour::transform(const Transform2F& form) {
		if (form.isIdentity()) {
			return;
		}

		for (Point & point : points) {
			point.point = form.apply(point.point);
		}
	}
	Contour Contour::transformed(const Transform2F& form) const {
		Contour copy = *this;
		copy.transform(form);
		return copy;
	}

	Contour::iterator Contour::begin() noexcept {
		return points.begin();
	}
	Contour::iterator Contour::end() noexcept {
		return points.end();
	}

	Contour::const_iterator Contour::begin() const noexcept {
		return points.begin();
	}
	Contour::const_iterator Contour::end() const noexcept {
		return points.end();
	}

	Contour::const_iterator Contour::cbegin() const noexcept {
		return points.cbegin();
	}
	Contour::const_iterator Contour::cend() const noexcept {
		return points.cend();
	}

	Contour::reverse_iterator Contour::rbegin() noexcept {
		return points.rbegin();
	}
	Contour::reverse_iterator Contour::rend() noexcept {
		return points.rend();
	}

	Contour::const_reverse_iterator Contour::rbegin() const noexcept {
		return points.rbegin();
	}
	Contour::const_reverse_iterator Contour::rend() const noexcept {
		return points.rend();
	}

	Contour::const_reverse_iterator Contour::crbegin() const noexcept {
		return points.crbegin();
	}
	Contour::const_reverse_iterator Contour::crend() const noexcept {
		return points.crend();
	}
};