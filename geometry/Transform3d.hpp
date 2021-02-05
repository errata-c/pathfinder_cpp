#pragma once
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quat.hpp>

namespace pf {
	struct Transform3F {
		Transform3F(const Transform3F&) noexcept = default;
		Transform3F& operator=(const Transform3F&) noexcept = default;
		~Transform3F() = default;

		Transform3F() noexcept;

		Transform3F lerp(const Transform3F& other, float t) const noexcept;
		glm::mat2 upperLeft() const noexcept;
		glm::mat2 upperRight() const noexcept;
		glm::mat2 lowerLeft() const noexcept;
		glm::mat2 lowerRight() const noexcept;

		Transform3F inverse() const noexcept;

		Transform3F rotate(const glm::vec3& angles) const noexcept;
		Transform3F rotate(const glm::quat& rot) const noexcept;
		Transform3F scale(float factor) const noexcept;
		Transform3F scale(const glm::vec3& factor) const noexcept;
		Transform3F translate(const glm::vec3& offset) const noexcept;

		bool approxEq(const Transform3F& other, float eps) const noexcept;

		Transform3F apply(const Transform3F& val) const noexcept;
		glm::vec3 apply(const glm::vec3& val) const noexcept;

		static Transform3F rowMajor(const glm::vec4 & r0, const glm::vec4& r1, const glm::vec4& r2, const glm::vec4& r3) noexcept;
		static Transform3F columnMajor(const glm::vec4& c0, const glm::vec4& c1, const glm::vec4& c2, const glm::vec4& c3) noexcept;
	
		static Transform3F fromScale(const glm::vec3& val) noexcept;
		static Transform3F fromScale(float val) noexcept;
		static Transform3F fromRotation(const glm::vec3& val) noexcept;
		static Transform3F fromTranslation(const glm::vec3& val) noexcept;
		static Transform3F fromOrtho(float left, float right, float top, float bottom, float near, float far) noexcept;
		static Transform3F fromPerspective(float fovY, float aspect, float near, float far) noexcept;
		static Transform3F fromLook(const glm::vec3 & eye, const glm::vec3 & center, const glm::vec3 & up) noexcept;
	};

	struct Perspective {
		Transform3F transform;
		glm::ivec2 viewportSize;
	};
};