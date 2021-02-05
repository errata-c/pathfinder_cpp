#include "Rect.hpp"
#include <glm/common.hpp>

namespace pf {
	RectF RectF::fromPoints(const glm::vec2& upper_left, const glm::vec2& lower_right) noexcept {
		return RectF{ glm::min(upper_left, lower_right), glm::max(lower_right, upper_left) };
	}

	RectF::RectF() noexcept
		: glm::vec4{ 0 }
	{}
	RectF::RectF(const glm::vec2& _origin, const glm::vec2& _size) noexcept
		: glm::vec4{_origin, _origin + _size}
	{}
	RectF::RectF(const RectI& other) noexcept
		: glm::vec4{other.origin(), other.size()}
	{}

	glm::vec2 RectF::origin() const {
		return glm::vec2{x, y};
	}
	glm::vec2 RectF::size() const {
		return lowerRight() - origin();
	}

	float RectF::width() const {
		return z - x;
	}
	float RectF::height() const {
		return w - y;
	}

	glm::vec2 RectF::upperRight() const {
		return glm::vec2{z, y};
	}
	glm::vec2 RectF::upperLeft() const {
		return glm::vec2{x, y};
	}

	glm::vec2 RectF::lowerRight() const {
		return glm::vec2{z, w};
	}
	glm::vec2 RectF::lowerLeft() const {
		return glm::vec2{x, w};
	}

	bool RectF::contains(const glm::vec2& point) const {
		return
			(point.x >= x) &&
			(point.y >= y) &&
			(point.x <= z) &&
			(point.y <= w);
	}
	bool RectF::contains(const RectF& rect) const {
		return
			(rect.x >= x) &&
			(rect.y >= y) &&
			(rect.z <= z) &&
			(rect.w <= w);
	}
	bool RectF::isEmpty() const {
		return upperLeft() == lowerRight();
	}

	RectF RectF::enclose(const glm::vec2& point) const {
		return RectF::fromPoints(
			glm::min(origin(), point),
			glm::max(lowerRight(), point)
		);
	}
	RectF RectF::enclose(const RectF& rect) const {
		return RectF::fromPoints(
			glm::min(origin(), rect.origin()),
			glm::max(lowerRight(), rect.lowerRight())
		);
	}

	bool RectF::intersects(const RectF& rect) const {
		return
			(rect.z > x) &&
			(rect.w > y) &&
			(rect.x < z) &&
			(rect.y < w);
	}
	std::optional<RectF> RectF::intersection(const RectF& rect) const {
		if (intersects(rect)) {
			return RectF::fromPoints(
				glm::max(origin(), rect.origin()),
				glm::min(lowerRight(), rect.lowerRight())
			);
		}
		else {
			return {};
		}
	}

	glm::vec2 RectF::center() const {
		return (lowerRight() + upperLeft()) * 0.5f;
	}

	RectF RectF::round() const {
		return RectF::fromPoints(
			glm::round(origin()),
			glm::round(lowerRight())
		);
	}
	RectF RectF::roundOut() const {
		return RectF::fromPoints(
			glm::floor(origin()),
			glm::ceil(lowerRight())
		);
	}
	RectF RectF::dilate(float amount) const {
		return dilate(glm::vec2{ amount });
	}
	RectF RectF::dilate(const glm::vec2& amount) const {
		return RectF::fromPoints(
			origin() - amount,
			lowerRight() + amount
		);
	}
	RectF RectF::contract(float amount) const {
		return contract(glm::vec2{ amount });
	}
	RectF RectF::contract(const glm::vec2& amount) const {
		return RectF::fromPoints(
			origin() + amount,
			lowerRight() - amount
		);
	}
};