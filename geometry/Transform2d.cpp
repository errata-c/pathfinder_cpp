#include "Transform2d.hpp"
#include "LineSegment.hpp"
#include "Rect.hpp"
#include <cmath>

namespace pf {
	Transform2F::Transform2F() noexcept 
		: matrix(1.f)
		, vector(0.f)
	{}
	Transform2F::Transform2F(const glm::mat2 & _matrix, const glm::vec2& _vector) noexcept
		: matrix(_matrix)
		, vector(_vector)
	{}

	bool Transform2F::isIdentity() const noexcept {
		return matrix == glm::mat2(1.f) && vector == glm::vec2(0.f);
	}
	glm::vec2 Transform2F::getScale() const noexcept {
		return glm::vec2{ matrix[0][0], matrix[1][1] };
	}
	float Transform2F::getScaleFactor() const noexcept {
		return glm::length(getScale());
	}
	float Transform2F::getRotation() const noexcept {
		return std::atan2(matrix[1][0], matrix[0][0]);
	}
	glm::vec2 Transform2F::getTranslation() const noexcept {
		return vector;
	}

	Transform2F Transform2F::scale(const glm::vec2& factor) const noexcept {
		Transform2F tmp = *this;
		tmp.matrix[0] *= factor;
		tmp.matrix[1] *= factor;
		return tmp;
	}
	Transform2F Transform2F::scale(float factor) const noexcept {
		return scale(glm::vec2{factor, factor});
	}
	Transform2F Transform2F::rotate(float angle) const noexcept {
		return Transform2F::fromRotation(angle).apply(*this);
	}
	Transform2F Transform2F::translate(const glm::vec2& amount) const noexcept {
		Transform2F tmp = *this;
		tmp.vector += amount;
		return tmp;
	}

	Transform2F Transform2F::inverse() const noexcept {
		glm::mat2 mat = glm::inverse(matrix);
		glm::vec2 vec = -mat * vector;
		return Transform2F{ mat, vec };
	}

	glm::vec2 Transform2F::apply(const glm::vec2& val) const noexcept {
		return matrix * val + vector;
	}
	LineSegment2F Transform2F::apply(const LineSegment2F& val) const noexcept {
		return LineSegment2F{ apply(val.from()), apply(val.to()) };
	}
	RectF Transform2F::apply(const RectF& val) const noexcept {
		glm::vec2 upperLeft = apply(val.origin()), upperRight = apply(val.upperRight());
		glm::vec2 lowerLeft = apply(val.lowerLeft()), lowerRight = apply(val.lowerRight());

		glm::vec2 minPoint = glm::min(upperLeft, glm::min(upperRight, glm::min(lowerLeft, lowerRight)));
		glm::vec2 maxPoint = glm::max(upperLeft, glm::max(upperRight, glm::max(lowerLeft, lowerRight)));
		return RectF::fromPoints(minPoint, maxPoint);
	}
	Transform2F Transform2F::apply(const Transform2F& val) const noexcept {
		return Transform2F{ matrix * val.matrix, apply(val.vector) };
	}

	Transform2F Transform2F::rowMajor(const glm::vec3& row0, const glm::vec3& row1) noexcept {
		glm::mat2 mat{ row0.x, row1.x, row0.y, row1.y };
		glm::vec2 vec{ row0.z, row1.z };
		return Transform2F{ mat, vec };
	}
	Transform2F Transform2F::columnMajor(const glm::vec2& col0, const glm::vec2& col1, const glm::vec2& col2) noexcept {
		return Transform2F{ glm::mat2{col0, col1}, col2 };
	}

	Transform2F Transform2F::fromScale(const glm::vec2& val) noexcept {
		return Transform2F{ glm::mat2{val.x, 0.f, 0.f, val.y}, glm::vec2{0.f} };
	}
	Transform2F Transform2F::fromScale(float val) noexcept {
		return fromScale(glm::vec2{ val, val });
	}
	Transform2F Transform2F::fromRotation(float val) noexcept {
		return fromRotation(glm::vec2{std::cos(val), std::sin(val)});
	}
	Transform2F Transform2F::fromRotation(const glm::vec2& val) noexcept {
		glm::mat2 mat{ val, glm::vec2{-val.y, val.x} };
		return Transform2F{ mat, glm::vec2{0.f} };
	}
	Transform2F Transform2F::fromTranslation(const glm::vec2& val) noexcept {
		return Transform2F{ glm::mat2{1.f}, val };
	}
	Transform2F Transform2F::fromScaleRotationTranslation(const glm::vec2& scale, float rotation, const glm::vec2& translation) noexcept {
		return fromTranslation(translation).apply(fromRotation(rotation).apply(fromScale(scale)));
	}

};