#pragma once
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <optional>

namespace pf {
	struct RectI;

	struct RectF {
		static RectF fromPoints(const glm::vec2 & _origin, const glm::vec2 & lower_right) noexcept;
		static RectF fromOriginSize(const glm::vec2& _origin, const glm::vec2& size) noexcept;

		RectF() noexcept;
		RectF(const glm::vec2& _origin, const glm::vec2& _size) noexcept;
		RectF(const RectI&) noexcept;
		RectF(const glm::vec4& other) noexcept;

		glm::vec2 origin() const;
		glm::vec2 size() const;

		float width() const;
		float height() const;

		glm::vec2 upperRight() const;
		glm::vec2 upperLeft() const;

		glm::vec2 lowerRight() const;
		glm::vec2 lowerLeft() const;

		glm::vec2 min() const noexcept;
		glm::vec2 max() const noexcept;
		float minX() const noexcept;
		float minY() const noexcept;
		float maxX() const noexcept;
		float maxY() const noexcept;

		bool contains(const glm::vec2& point) const;
		bool contains(const RectF& rect) const;
		bool isEmpty() const;

		RectF merge(const glm::vec2& point) const;
		RectF merge(const RectF& rect) const;

		bool intersects(const RectF& rect) const;
		std::optional<RectF> intersection(const RectF & rect) const;

		glm::vec2 center() const;

		RectF round() const;
		RectF roundOut() const;
		RectF dilate(float amount) const;
		RectF dilate(const glm::vec2& amount) const;
		RectF contract(float amount) const;
		RectF contract(const glm::vec2& amount) const;

		glm::vec4& asVec4() noexcept;
		const glm::vec4& asVec4() const noexcept;
	private:
		glm::vec4 data;
	};
};

pf::RectF operator+(const pf::RectF& lh, const glm::vec2& rh) noexcept;
pf::RectF operator+=(const pf::RectF& lh, const glm::vec2& rh) noexcept;
pf::RectF operator+(const pf::RectF& lh, float rh) noexcept;
pf::RectF operator+=(const pf::RectF& lh, float rh) noexcept;

pf::RectF operator-(const pf::RectF& lh, const glm::vec2& rh) noexcept;
pf::RectF operator-=(const pf::RectF& lh, const glm::vec2& rh) noexcept;
pf::RectF operator-(const pf::RectF& lh, float rh) noexcept;
pf::RectF operator-=(const pf::RectF& lh, float rh) noexcept;

pf::RectF operator*(const pf::RectF& lh, const glm::vec2& rh) noexcept;
pf::RectF operator*=(const pf::RectF& lh, const glm::vec2& rh) noexcept;
pf::RectF operator*(const pf::RectF& lh, float rh) noexcept;
pf::RectF operator*=(const pf::RectF& lh, float rh) noexcept;


namespace pf {
	struct RectI {
		static RectI fromPoints(const glm::ivec2& _origin, const glm::ivec2& _lowerRight) noexcept;
		static RectI fromOriginSize(const glm::ivec2& _origin, const glm::ivec2& size) noexcept;

		RectI(const glm::ivec2& _origin, const glm::ivec2& _lowerRight) noexcept;
		RectI(const RectF& other) noexcept;
		RectI(const glm::ivec4& other) noexcept;

		glm::ivec2 origin() const;
		glm::ivec2 size() const;

		int width() const;
		int height() const;
		int area() const;

		glm::ivec2 upperRight() const;
		glm::ivec2 upperLeft() const;

		glm::ivec2 lowerRight() const;
		glm::ivec2 lowerLeft() const;

		glm::ivec2 min() const noexcept;
		glm::ivec2 max() const noexcept;
		int minX() const noexcept;
		int minY() const noexcept;
		int maxX() const noexcept;
		int maxY() const noexcept;

		RectI scale(int factor) const;
		RectI scaleXY(glm::ivec2 factor) const;

		bool contains(const glm::ivec2& point) const noexcept;
		bool contains(const RectI& rect) const noexcept;
		bool isEmpty() const noexcept;

		RectI merge(const glm::ivec2& point) const;
		RectI merge(const RectI& rect) const;

		bool intersects(const RectI& rect) const;
		std::optional<RectI> intersection(const RectI& rect) const;

		glm::ivec2 center() const;

		RectI contract(const glm::ivec2& amount) const;

		glm::ivec4& asVec4() noexcept;
		const glm::ivec4& asVec4() const noexcept;
	private:
		glm::ivec4 data;
	};
}

// What are these supposed to do?
/*
pf::RectI operator*(const pf::RectI& lh, const glm::ivec2& rh) noexcept;
pf::RectI operator*=(const pf::RectI& lh, const glm::ivec2& rh) noexcept;
pf::RectI operator*(const pf::RectI& lh, int rh) noexcept;
pf::RectI operator*=(const pf::RectI& lh, int rh) noexcept;
*/