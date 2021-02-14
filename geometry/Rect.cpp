#include "Rect.hpp"
#include <glm/common.hpp>

namespace pf {
	RectF RectF::fromPoints(const glm::vec2& upper_left, const glm::vec2& lower_right) noexcept {
		return RectF{ glm::min(upper_left, lower_right), glm::max(lower_right, upper_left) };
	}
	RectF RectF::fromOriginSize(const glm::vec2& upperLeft, const glm::vec2& size) noexcept {
		return RectF(upperLeft, upperLeft + size);
	}

	RectF::RectF() noexcept
		: data{ 0.f }
	{}
	RectF::RectF(const glm::vec2& _upperLeft, const glm::vec2& _lowerRight) noexcept
		: data{ _upperLeft, _lowerRight }
	{}
	RectF::RectF(const RectI& other) noexcept
		: data{other.min(), other.max()}
	{}

	glm::vec2 RectF::origin() const {
		return min();
	}
	glm::vec2 RectF::size() const {
		return max() - min();
	}

	float RectF::width() const {
		return maxX() - minX();
	}
	float RectF::height() const {
		return maxY() - minY();
	}

	glm::vec2 RectF::upperRight() const {
		return glm::vec2{ data.z, data.y };
	}
	glm::vec2 RectF::upperLeft() const {
		return glm::vec2{ data.x, data.y };
	}

	glm::vec2 RectF::lowerRight() const {
		return glm::vec2{ data.z, data.w };
	}
	glm::vec2 RectF::lowerLeft() const {
		return glm::vec2{ data.x, data.w };
	}

	glm::vec2 RectF::min() const noexcept {
		return upperLeft();
	}
	glm::vec2 RectF::max() const noexcept {
		return lowerRight();
	}
	float RectF::minX() const noexcept {
		return data.x;
	}
	float RectF::minY() const noexcept {
		return data.y;
	}
	float RectF::maxX() const noexcept {
		return data.z;
	}
	float RectF::maxY() const noexcept {
		return data.w;
	}

	bool RectF::contains(const glm::vec2& point) const {
		return
			(point.x >= data.x) &&
			(point.y >= data.y) &&
			(point.x <= data.z) &&
			(point.y <= data.w);
	}
	bool RectF::contains(const RectF& rect) const {
		return
			(rect.data.x >= data.x) &&
			(rect.data.y >= data.y) &&
			(rect.data.z <= data.z) &&
			(rect.data.w <= data.w);
	}
	bool RectF::isEmpty() const {
		return upperLeft() == lowerRight();
	}

	RectF RectF::merge(const glm::vec2& point) const {
		return RectF::fromPoints(
			glm::min(origin(), point),
			glm::max(lowerRight(), point)
		);
	}
	RectF RectF::merge(const RectF& rect) const {
		return RectF::fromPoints(
			glm::min(upperLeft(), rect.upperLeft()),
			glm::max(lowerRight(), rect.lowerRight())
		);
	}

	bool RectF::intersects(const RectF& rect) const {
		return
			(rect.data.z > data.x) &&
			(rect.data.w > data.y) &&
			(rect.data.x < data.z) &&
			(rect.data.y < data.w);
	}
	std::optional<RectF> RectF::intersection(const RectF& rect) const {
		if (intersects(rect)) {
			return RectF::fromPoints(
				glm::max(upperLeft(), rect.upperLeft()),
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
			glm::round(upperLeft()),
			glm::round(lowerRight())
		);
	}
	RectF RectF::roundOut() const {
		return RectF::fromPoints(
			glm::floor(upperLeft()),
			glm::ceil(lowerRight())
		);
	}
	RectF RectF::dilate(float amount) const {
		return dilate(glm::vec2{ amount });
	}
	RectF RectF::dilate(const glm::vec2& amount) const {
		return RectF::fromPoints(
			upperLeft() - amount,
			lowerRight() + amount
		);
	}
	RectF RectF::contract(float amount) const {
		return contract(glm::vec2{ amount });
	}
	RectF RectF::contract(const glm::vec2& amount) const {
		return RectF::fromPoints(
			upperLeft() + amount,
			lowerRight() - amount
		);
	}

	glm::vec4& RectF::asVec4() noexcept {
		return data;
	}
	const glm::vec4& RectF::asVec4() const noexcept {
		return data;
	}
};

namespace pf {
	RectI RectI::fromPoints(const glm::ivec2& upperLeft, const glm::ivec2& lowerRight) noexcept {
		return RectI(upperLeft, lowerRight);
	}
	RectI RectI::fromOriginSize(const glm::ivec2& upperLeft, const glm::ivec2& size) noexcept {
		return RectI(upperLeft, upperLeft + size);
	}

	RectI::RectI(const glm::ivec2& upperLeft, const glm::ivec2& lowerRight) noexcept
		: data{upperLeft, lowerRight}
	{}
	RectI::RectI(const RectF& other) noexcept 
		: data{other.upperLeft(), other.lowerRight()}
	{}
	RectI::RectI(const glm::ivec4& other) noexcept 
		: data{other}
	{}

	glm::ivec2 RectI::origin() const {
		return upperLeft();
	}
	glm::ivec2 RectI::size() const {
		return lowerRight() - upperLeft();
	}
	int RectI::area() const {
		return width() * height();
	}
	int RectI::width() const {
		return data.z - data.x;
	}
	int RectI::height() const {
		return data.w - data.y;
	}

	glm::ivec2 RectI::upperRight() const {
		return glm::ivec2{ data.z, data.y };
	}
	glm::ivec2 RectI::upperLeft() const {
		return glm::ivec2{ data.x, data.y };
	}

	glm::ivec2 RectI::lowerRight() const {
		return glm::ivec2{ data.z, data.w };
	}
	glm::ivec2 RectI::lowerLeft() const {
		return glm::ivec2{ data.x, data.w };
	}

	glm::ivec2 RectI::min() const noexcept {
		return upperLeft();
	}
	glm::ivec2 RectI::max() const noexcept {
		return lowerRight();
	}
	int RectI::minX() const noexcept {
		return data.x;
	}
	int RectI::minY() const noexcept {
		return data.y;
	}
	int RectI::maxX() const noexcept {
		return data.z;
	}
	int RectI::maxY() const noexcept {
		return data.w;
	}

	RectI RectI::scale(int factor) const {
		return RectI(data * factor);
	}
	RectI RectI::scaleXY(glm::ivec2 factor) const {
		return RectI(data * glm::ivec4{ factor, factor });
	}

	bool RectI::contains(const glm::ivec2& point) const noexcept {
		return
			(point.x >= minX()) &&
			(point.y >= minY()) &&
			(point.x < maxX()) &&
			(point.y < maxY());
	}
	bool RectI::contains(const RectI& rect) const noexcept {
		return
			(rect.minX() >= minX()) &&
			(rect.minX() >= minY()) &&
			(rect.maxX() < maxX()) &&
			(rect.maxY() < maxY());
	}

	bool RectI::isEmpty() const noexcept {
		return min() == max();
	}

	RectI RectI::merge(const glm::ivec2& point) const {
		return RectI{
			glm::min(point, min()),
			glm::max(point, max())
		};
	}
	RectI RectI::merge(const RectI& rect) const {
		return RectI{
			glm::min(min(), rect.min()),
			glm::max(max(), rect.max())
		};
	}

	bool RectI::intersects(const RectI& rect) const {
		return
			(rect.maxX() >= minX()) &&
			(rect.maxY() >= minY()) &&
			(rect.minX() < maxX()) &&
			(rect.minY() < maxY());
	}
	std::optional<RectI> RectI::intersection(const RectI& rect) const {
		if (intersects(rect)) {
			return RectI{
				glm::max(upperLeft(), rect.upperLeft()),
				glm::min(lowerRight(), rect.lowerRight())
			};
		}
		else {
			return std::nullopt;
		}
	}

	glm::ivec2 RectI::center() const {
		return (min() + max()) >> 1;
	}

	RectI RectI::contract(const glm::ivec2& amount) const {
		return RectI{
			upperLeft() + amount,
			lowerRight() - amount
		};
	}
};