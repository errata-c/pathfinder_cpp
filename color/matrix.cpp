#include "matrix.hpp"
#include <cmath>

namespace pf {
	ColorMatrix::ColorMatrix() noexcept
		: std::array<glm::vec4, 5>{
			glm::vec4{ 0.f },
			glm::vec4{ 0.f },
			glm::vec4{ 0.f },
			glm::vec4{ 0.f },
			glm::vec4{ 0.f }
		}
	{}

	ColorMatrix::ColorMatrix(const glm::vec4& c0, const glm::vec4& c1, const glm::vec4& c2, const glm::vec4& c3, const glm::vec4& c4) noexcept
		: std::array<glm::vec4, 5>{
			c0, c1, c2, c3, c4
		}
	{}

	ColorMatrix ColorMatrix::from_rows(const std::array<float, 5>& r0, const std::array<float, 5>& r1, const std::array<float, 5>& r2, const std::array<float, 5>& r3) noexcept {
		return ColorMatrix{
			glm::vec4{ r0[0], r1[0], r2[0], r3[0] },
			glm::vec4{ r0[1], r1[1], r2[1], r3[1] },
			glm::vec4{ r0[2], r1[2], r2[2], r3[2] },
			glm::vec4{ r0[3], r1[3], r2[3], r3[3] },
			glm::vec4{ r0[4], r1[4], r2[4], r3[4] }
		};
	}

	ColorMatrix ColorMatrix::hue_rotate(float angle) noexcept {
		ColorMatrix a = ColorMatrix::from_rows(
			{ 0.213, 0.715, 0.072, 0.0, 0.0 },
			{ 0.213, 0.715, 0.072, 0.0, 0.0 },
			{ 0.213, 0.715, 0.072, 0.0, 0.0 },
			{ 0.0, 0.0, 0.0, 1.0, 0.0 }
		);
		ColorMatrix b = ColorMatrix::from_rows(
			{ 0.787, -0.715, -0.072, 0.0, 0.0 },
			{ -0.213, 0.285, -0.072, 0.0, 0.0 },
			{ -0.213, -0.715, 0.928, 0.0, 0.0 },
			{ 0.0, 0.0, 0.0, 0.0, 0.0 }
		);
		ColorMatrix c = ColorMatrix::from_rows(
			{ -0.213, -0.715, 0.928, 0.0, 0.0 },
			{ 0.143, 0.140, -0.283, 0.0, 0.0 },
			{ -0.787, 0.715, 0.072, 0.0, 0.0 },
			{ 0.0, 0.0, 0.0, 0.0, 0.0 }
		);
		return a + b * std::cos(angle) + c * std::sin(angle);
	}

	ColorMatrix ColorMatrix::saturate(float saturation) noexcept {
		ColorMatrix a = ColorMatrix::from_rows(
			{ 0.213, 0.715, 0.072, 0.0, 0.0 },
			{ 0.213, 0.715, 0.072, 0.0, 0.0 },
			{ 0.213, 0.715, 0.072, 0.0, 0.0 },
			{ 0.0, 0.0, 0.0, 1.0, 0.0 }
		);
		ColorMatrix b = ColorMatrix::from_rows(
			{ 0.787, -0.715, -0.072, 0.0, 0.0 },
			{ -0.213, 0.285, -0.072, 0.0, 0.0 },
			{ -0.213, -0.715, 0.928, 0.0, 0.0 },
			{ 0.0, 0.0, 0.0, 0.0, 0.0 }
		);
		return a + b * saturation;
	}

	ColorMatrix ColorMatrix::luminance_to_alpha() noexcept {
		return ColorMatrix::from_rows(
			{ 0.0, 0.0, 0.0, 0.0, 0.0 },
			{ 0.0, 0.0, 0.0, 0.0, 0.0 },
			{ 0.0, 0.0, 0.0, 0.0, 0.0 },
			{ 0.2125, 0.7154, 0.0721, 0.0, 0.0 }
		);
	}
}

pf::ColorMatrix operator+(const pf::ColorMatrix& lh, const pf::ColorMatrix& rh) {
	return pf::ColorMatrix{
		lh[0] + rh[0],
		lh[1] + rh[1],
		lh[2] + rh[2],
		lh[3] + rh[3],
		lh[4] + rh[4]
	};
}
pf::ColorMatrix operator-(const pf::ColorMatrix& lh, const pf::ColorMatrix& rh) {
	return pf::ColorMatrix{
		lh[0] - rh[0],
		lh[1] - rh[1],
		lh[2] - rh[2],
		lh[3] - rh[3],
		lh[4] - rh[4]
	};
}
pf::ColorMatrix operator*(const pf::ColorMatrix& lh, float rh) {
	return pf::ColorMatrix{
		lh[0] * rh,
		lh[1] * rh,
		lh[2] * rh,
		lh[3] * rh,
		lh[4] * rh
	};
}