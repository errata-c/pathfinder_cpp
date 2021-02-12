#pragma once
#include "../geometry/LineSegment.hpp"
#include "../geometry/Transform2d.hpp"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat2x2.hpp>
#include <functional>
#include <cmath>

namespace std {
	template<>
	struct hash<glm::vec2> {
		std::size_t operator()(glm::vec2 const& val) const noexcept {
			std::size_t
				h1 = std::hash<float>{}(val.x),
				h2 = std::hash<float>{}(val.y);
			return h1 ^ (h2 << 1);
		}
	};
	template<>
	struct hash<glm::vec3> {
		std::size_t operator()(glm::vec3 const& val) const noexcept {
			std::size_t
				h1 = std::hash<float>{}(val.x),
				h2 = std::hash<float>{}(val.y),
				h3 = std::hash<float>{}(val.z);
			h1 = h1 ^ (h2 << 1);
			return h1 ^ (h3 << 1);
		}
	};
	template<>
	struct hash<glm::vec4> {
		std::size_t operator()(glm::vec4 const& val) const noexcept {
			std::size_t
				h1 = std::hash<float>{}(val.x),
				h2 = std::hash<float>{}(val.y),
				h3 = std::hash<float>{}(val.z),
				h4 = std::hash<float>{}(val.w);
			h1 = h1 ^ (h2 << 1);
			h1 = h1 ^ (h3 << 1);
			return h1 ^ (h4 << 1);
		}
	};

	template<>
	struct hash<pf::LineSegment2F> {
		std::size_t operator()(pf::LineSegment2F const& val) const noexcept {
			return std::hash<glm::vec4>{}(val);
		}
	};

	template<>
	struct hash<pf::Transform2F> {
		std::size_t operator()(pf::Transform2F const& val) const noexcept {
			std::size_t
				h1 = std::hash<glm::vec2>{}(val.matrix[0]),
				h2 = std::hash<glm::vec2>{}(val.matrix[1]),
				h3 = std::hash<glm::vec2>{}(val.vector);
			h1 = h1 ^ (h2 << 1);
			return h1 ^ (h3 << 1);
		}
	};
};

namespace pf {
	template<typename T>
	T safeSqrt(const T& val) noexcept {
		if (val < static_cast<T>(0)) {
			return static_cast<T>(0);
		}
		else {
			return std::sqrt(val);
		}
	}

	// rotate v1 by v2
	static glm::vec2 rotateBy(const glm::vec2& v1, const glm::vec2& v2) {
		return glm::mat2{ v2.x, v2.y, -v2.y, v2.x } * v1;
	}
	// reverse rotate v1 by v2
	static glm::vec2 revRotateBy(const glm::vec2& v1, const glm::vec2& v2) {
		return glm::mat2{ v2.x, -v2.y, v2.y, v2.x } * v1;
	}

	static glm::vec2 halveAngle(const glm::vec2& v) {
		glm::vec2 term{v.x, -v.x};
		return glm::sqrt(glm::max(0.5f * (1.f + term), glm::vec2{ 0.f }));
	}

	static constexpr float
		pi = 3.141592653,
		tau = 6.283185307,
		root2 = 1.414213562;
};