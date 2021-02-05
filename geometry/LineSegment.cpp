#include "LineSegment.hpp"
#include <glm/gtx/compatibility.hpp>

namespace pf {
	LineSegment2F::LineSegment2F() noexcept
		: glm::vec4(0.f)
	{}
	LineSegment2F::LineSegment2F(const glm::vec2& from, const glm::vec2& to) noexcept
		: glm::vec4(from, to)
	{}

	glm::vec2 LineSegment2F::from() const noexcept {
		return glm::vec2(x, y);
	}
	glm::vec2 LineSegment2F::to() const noexcept {
		return glm::vec2(z, w);
	}
	float LineSegment2F::toX() const noexcept {
		return x;
	}
	float LineSegment2F::toY() const noexcept {
		return y;
	}
	float LineSegment2F::fromX() const noexcept {
		return z;
	}
	float LineSegment2F::fromY() const noexcept {
		return w;
	}

	void LineSegment2F::setFrom(const glm::vec2& val) noexcept {
		x = val.x;
		y = val.y;
	}
	void LineSegment2F::setTo(const glm::vec2& val) noexcept {
		w = val.x;
		z = val.y;
	}
	std::array<LineSegment2F, 2> LineSegment2F::split(float t) const noexcept {
		glm::vec2 tmp = sample(t);
		return { LineSegment2F{from(), tmp}, LineSegment2F{tmp, to()} };
	}

	// Returns the left segment first, followed by the right segment.
	std::array<LineSegment2F, 2> LineSegment2F::splitX(float _x) const noexcept {
		glm::vec2 tmp = sample(solveTX(_x));
		if(fromX() < tmp.x) {
			return { LineSegment2F{from(), tmp}, LineSegment2F{tmp, to()} };
		}
		else {
			return { LineSegment2F{tmp, to()}, LineSegment2F{from(), tmp} };
		}
	}


	std::array<LineSegment2F, 2> LineSegment2F::splitY(float _y) const noexcept {
		glm::vec2 tmp = sample(solveTY(_y));
		// Make sure we compare `from_y` and `to_y` to properly handle the case in which one of the
		// two segments is zero-length.
		if (fromY() < toY()) {
			return { LineSegment2F{from(), tmp}, LineSegment2F{tmp, to()} };
		}
		else {
			return { LineSegment2F{tmp, to()}, LineSegment2F{from(), tmp} };
		}
	}

	float LineSegment2F::solveTX(float _x) const noexcept {
		return (_x - fromX()) / (toX() - fromX());
	}
	float LineSegment2F::solveTY(float _y) const noexcept {
		return (_y - fromY()) / (toY() - fromY());
	}
	float LineSegment2F::solveXY(float _y) const noexcept {
		return glm::lerp(fromX(), toX(), solveTY(_y));
	}
	float LineSegment2F::solveYX(float _x) const noexcept {
		return glm::lerp(fromY(), toY(), solveTX(_x));
	}

	LineSegment2F LineSegment2F::reversed() const noexcept {
		return LineSegment2F{to(), from()};
	}

	glm::vec2 LineSegment2F::upperPoint() const noexcept {
		if(fromY() < toY()) {
			return from();
		}
		else {
			return to();
		}
	}
	glm::vec2 LineSegment2F::lowerPoint() const noexcept {
		if (fromY() < toY()) {
			return to();
		}
		else {
			return from();
		}
	}

	int LineSegment2F::windingY() const noexcept {
		if (fromY() < toY()) {
			return 1;
		}
		else {
			return -1;
		}
	}
	int LineSegment2F::windingX() const noexcept {
		if (fromX() < toX()) {
			return 1;
		}
		else {
			return -1;
		}
	}
	LineSegment2F LineSegment2F::orient(int winding) const noexcept {
		if (winding >= 0) {
			return *this;
		}
		else {
			return reversed();
		}
	}

	float LineSegment2F::length() const noexcept {
		return glm::length(vector());
	}
	float LineSegment2F::length2() const noexcept {
		glm::vec2 tmp = vector();
		return glm::dot(tmp, tmp);
	}

	glm::vec2 LineSegment2F::vector() const noexcept {
		return to() - from();
	}

	glm::vec2 LineSegment2F::sample(float t) const noexcept {
		return glm::lerp(from(), to(), t);
	}
	glm::vec2 LineSegment2F::midpoint() const noexcept {
		return (to() + from()) * 0.5f;
	}

	LineSegment2F LineSegment2F::offset(float amount) const noexcept {
		if (isZeroLength()) {
			return *this;
		}
		else {
			glm::vec2 tmp = vector();
			tmp = glm::normalize(glm::vec2(-tmp.y, tmp.x)) * amount;

			return LineSegment2F(from() + tmp, to() + tmp);
		}
	}
	bool LineSegment2F::isZeroLength() const noexcept {
		return length2() < 1e-5f;
	}

	std::optional<float> LineSegment2F::intersect(const LineSegment2F& other) const noexcept {
		glm::vec2 p0p1 = vector();
		glm::mat2 matrix{ other.vector(), -p0p1 };
		if(std::abs(glm::determinant(matrix)) < 1e-4f) {
			return {};
		}
		return { (glm::inverse(matrix) * (from() - other.from())).y };
	}
}