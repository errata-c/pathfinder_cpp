#pragma once
#include <glm/vec2.hpp>
#include <glm/mat2x2.hpp>

namespace pf {
	struct LineSegment2F;
	struct RectF;

	struct Transform2F {
		Transform2F(const Transform2F&) noexcept = default;
		Transform2F& operator=(const Transform2F&) noexcept = default;
		~Transform2F() = default;

		Transform2F() noexcept;
		Transform2F(const glm::mat2& _matrix, const glm::vec2& _vector) noexcept;

		bool isIdentity() const noexcept;
		glm::vec2 getScale() const noexcept;
		float getScaleFactor() const noexcept;
		float getRotation() const noexcept;
		glm::vec2 getTranslation() const noexcept;

		Transform2F scale(const glm::vec2&) const noexcept;
		Transform2F scale(float) const noexcept;
		Transform2F rotate(float angle) const noexcept;
		Transform2F translate(const glm::vec2& amount) const noexcept;

		Transform2F inverse() const noexcept;

		glm::vec2 apply(const glm::vec2& val) const noexcept;
		LineSegment2F apply(const LineSegment2F& val) const noexcept;
		RectF apply(const RectF& val) const noexcept;
		Transform2F apply(const Transform2F& val) const noexcept;

		static Transform2F rowMajor(const glm::vec3 & row0, const glm::vec3 & row1) noexcept;
		static Transform2F columnMajor(const glm::vec2& col0, const glm::vec2& col1, const glm::vec2& col2) noexcept;

		static Transform2F fromScale(const glm::vec2 & ) noexcept;
		static Transform2F fromScale(float ) noexcept;
		static Transform2F fromRotation(float angle) noexcept;
		static Transform2F fromRotation(const glm::vec2&) noexcept;
		static Transform2F fromTranslation(const glm::vec2&) noexcept;
		static Transform2F fromScaleRotationTranslation(const glm::vec2& scale, float rotation, const glm::vec2& translation) noexcept;
	
		glm::mat2 matrix;
		glm::vec2 vector;
	};
};