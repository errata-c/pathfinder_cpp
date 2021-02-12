#pragma once
#include <array>
#include <optional>

#include <glm/vec4.hpp>
#include <glm/mat2x2.hpp>


namespace pf {
	struct LineSegment2F: public glm::vec4 {
		LineSegment2F(const LineSegment2F&) noexcept = default;
		LineSegment2F& operator=(const LineSegment2F&) noexcept = default;
		~LineSegment2F() = default;

		LineSegment2F() noexcept;
		LineSegment2F(const glm::vec4&) noexcept;
		LineSegment2F(const glm::vec2& from, const glm::vec2& to) noexcept;

		glm::vec2 from() const noexcept;
		glm::vec2 to() const noexcept;
		float toX() const noexcept;
		float toY() const noexcept;
		float fromX() const noexcept;
		float fromY() const noexcept;

		void setFrom(const glm::vec2 & val) noexcept;
		void setTo(const glm::vec2& val) noexcept;

		std::array<LineSegment2F, 2> split(float t) const noexcept;
		std::array<LineSegment2F, 2> splitX(float x) const noexcept;
		std::array<LineSegment2F, 2> splitY(float y) const noexcept;

		LineSegment2F orient(int winding) const noexcept;

		float solveTX(float x) const noexcept;
		float solveTY(float y) const noexcept;
		float solveXY(float y) const noexcept;
		float solveYX(float x) const noexcept;

		LineSegment2F reversed() const noexcept;

		glm::vec2 upperPoint() const noexcept;
		glm::vec2 lowerPoint() const noexcept;

		int windingY() const noexcept;
		int windingX() const noexcept;

		float length() const noexcept;
		float length2() const noexcept;

		glm::vec2 vector() const noexcept;

		glm::vec2 sample(float t) const noexcept;
		glm::vec2 midpoint() const noexcept;

		LineSegment2F offset(float amount) const noexcept;
		bool isZeroLength() const noexcept;

		std::optional<float> intersect(const LineSegment2F& other) const noexcept;
	};
}

pf::LineSegment2F operator+(const pf::LineSegment2F& lh, const glm::vec2& rh) noexcept;
pf::LineSegment2F operator-(const pf::LineSegment2F& lh, const glm::vec2& rh) noexcept;
pf::LineSegment2F operator*(const pf::LineSegment2F& lh, const glm::vec2& rh) noexcept;
pf::LineSegment2F operator*(const pf::LineSegment2F& lh, float rh) noexcept;
pf::LineSegment2F& operator*=(pf::LineSegment2F& lh, const glm::vec2& rh) noexcept;